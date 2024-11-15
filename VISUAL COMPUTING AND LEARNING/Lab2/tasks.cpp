#include <unordered_map>

#include <glm/gtc/matrix_inverse.hpp>
#include <spdlog/spdlog.h>

#include "Labs/2-GeometryProcessing/DCEL.hpp"
#include "Labs/2-GeometryProcessing/tasks.h"

namespace VCX::Labs::GeometryProcessing {

#include "Labs/2-GeometryProcessing/marching_cubes_table.h"

    /******************* 1. Mesh Subdivision *****************/
    void SubdivisionMesh(Engine::SurfaceMesh const & input, Engine::SurfaceMesh & output, std::uint32_t numIterations) {
        Engine::SurfaceMesh curr_mesh = input;
        // We do subdivison iteratively.
        for (std::uint32_t it = 0; it < numIterations; ++it) {
            // During each iteration, we first move curr_mesh into prev_mesh.
            Engine::SurfaceMesh prev_mesh;
            prev_mesh.Swap(curr_mesh);
            // Then we create doubly connected edge list.
            DCEL G(prev_mesh);
            if (! G.IsManifold()) {
                spdlog::warn("VCX::Labs::GeometryProcessing::SubdivisionMesh(..): Non-manifold mesh.");
                return;
            }
            // Note that here curr_mesh has already been empty.
            // We reserve memory first for efficiency.
            curr_mesh.Positions.reserve(prev_mesh.Positions.size() * 3 / 2);
            curr_mesh.Indices.reserve(prev_mesh.Indices.size() * 4);
            // Then we iteratively update currently existing vertices.
            for (std::size_t i = 0; i < prev_mesh.Positions.size(); ++i) {
                // Update the currently existing vetex v from prev_mesh.Positions.
                // Then add the updated vertex into curr_mesh.Positions.
                auto v           = G.Vertex(i);
                auto neighbors   = v->Neighbors();
                // your code here:
                int n = neighbors.size();
                float u = n == 3 ? 3. / 16 : 3. / 8 / n;
                auto newpos = prev_mesh.Positions[i];
                newpos *= (1 - n * u);
                for (auto t : neighbors) {
                    newpos += u * prev_mesh.Positions[t];
                }
                curr_mesh.Positions.push_back(newpos);
            }
            // We create an array to store indices of the newly generated vertices.
            // Note: newIndices[i][j] is the index of vertex generated on the "opposite edge" of j-th
            //       vertex in the i-th triangle.
            std::vector<std::array<std::uint32_t, 3U>> newIndices(prev_mesh.Indices.size() / 3, { ~0U, ~0U, ~0U });
            // Iteratively process each halfedge.
            for (auto e : G.Edges()) {
                // newIndices[face index][vertex index] = index of the newly generated vertex
                newIndices[G.IndexOf(e->Face())][e->EdgeLabel()] = curr_mesh.Positions.size();
                auto eTwin                                       = e->TwinEdgeOr(nullptr);
                // eTwin stores the twin halfedge.
                if (! eTwin) {
                    // When there is no twin halfedge (so, e is a boundary edge):
                    // your code here: generate the new vertex and add it into curr_mesh.Positions.
                    curr_mesh.Positions.push_back((prev_mesh.Positions[e->To()] + prev_mesh.Positions[e->From()]) / (float)2.);
                } else {
                    // When the twin halfedge exists, we should also record:
                    //     newIndices[face index][vertex index] = index of the newly generated vertex
                    // Because G.Edges() will only traverse once for two halfedges,
                    //     we have to record twice.
                    newIndices[G.IndexOf(eTwin->Face())][e->TwinEdge()->EdgeLabel()] = curr_mesh.Positions.size();
                    // your code here: generate the new vertex and add it into curr_mesh.Positions.
                    glm::vec3 v0 = prev_mesh.Positions[e->To()];
                    glm::vec3 v2 = prev_mesh.Positions[e->From()];
                    glm::vec3 v1 = prev_mesh.Positions[e->OppositeVertex()];
                    glm::vec3 v3 = prev_mesh.Positions[e->TwinEdge()->OppositeVertex()];
                    glm::vec3 nv = (float) 3. / 8 * (v0 + v2) + (float) 1. / 8 * (v1 + v3);
                    curr_mesh.Positions.push_back(nv);
                }
            }

            // Here we've already build all the vertices.
            // Next, it's time to reconstruct face indices.
            for (std::size_t i = 0; i < prev_mesh.Indices.size(); i += 3U) {
                // For each face F in prev_mesh, we should create 4 sub-faces.
                // v0,v1,v2 are indices of vertices in F.
                // m0,m1,m2 are generated vertices on the edges of F.
                auto v0           = prev_mesh.Indices[i + 0U];
                auto v1           = prev_mesh.Indices[i + 1U];
                auto v2           = prev_mesh.Indices[i + 2U];
                auto [m0, m1, m2] = newIndices[i / 3U];
                // Note: m0 is on the opposite edge (v1-v2) to v0.
                // Please keep the correct indices order (consistent with order v0-v1-v2)
                //     when inserting new face indices.
                // toInsert[i][j] stores the j-th vertex index of the i-th sub-face.
                std::uint32_t toInsert[4][3] = {
                    // your code here:
                    {
                        v0, m2, m1
                    }, 
                    {
                        m2, v1, m0
                    }, 
                    {
                        m2, m0, m1
                    }, 
                    {
                        m1, m0, v2
                    }
                };
                // Do insertion.
                curr_mesh.Indices.insert(
                    curr_mesh.Indices.end(),
                    reinterpret_cast<std::uint32_t *>(toInsert),
                    reinterpret_cast<std::uint32_t *>(toInsert) + 12U
                );
            }

            if (curr_mesh.Positions.size() == 0) {
                spdlog::warn("VCX::Labs::GeometryProcessing::SubdivisionMesh(..): Empty mesh.");
                output = input;
                return;
            }
        }
        // Update output.
        output.Swap(curr_mesh);
    }

    /******************* 2. Mesh Parameterization *****************/
    void Parameterization(Engine::SurfaceMesh const & input, Engine::SurfaceMesh & output, const std::uint32_t numIterations) {
        // Copy.
        output = input;
        // Reset output.TexCoords.
        output.TexCoords.resize(input.Positions.size(), glm::vec2 { 0 });

        // Build DCEL.
        DCEL G(input);
        if (! G.IsManifold()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::Parameterization(..): non-manifold mesh.");
            return;
        }

        // Set boundary UVs for boundary vertices.
        // your code here: directly edit output.TexCoords
        std::vector < int > pot;
        std::vector < bool > mark(input.Positions.size(), 0);
        for(auto e : G.Edges()) {
            if(!e->TwinEdgeOr(nullptr)) {
                pot.push_back(e->From());
                pot.push_back(e->To());
                mark[e->From()] = mark[e->To()] = 1;
            }
        }
        sort(pot.begin(), pot.end(), [&](int x, int y) { 
            return atan2(input.Positions[x].x, input.Positions[x].y) < 
                   atan2(input.Positions[y].x, input.Positions[y].y);
        });
        for(int i = 0; i < pot.size(); i++) {
            output.TexCoords[pot[i]] = glm::vec2{ cos(2 * M_PI * i / pot.size()) / 2 + 1, sin(2 * M_PI * i / pot.size()) / 2 + 1 };
        }
        // Solve equation via Gauss-Seidel Iterative Method.
        for (int k = 0; k < numIterations; ++k) {
            // your code here:
            for(int i = 0; i < input.Positions.size(); i++) if(!mark[i]) {
                glm::vec2 nx(0, 0);//output.TexCoords[i];
                auto neigh = G.Vertex(i)->Neighbors();
                float ct = 1. / neigh.size();
                for (auto v : neigh) 
                    nx += output.TexCoords[v];
                nx *= ct; 
                // printf("%f %f\n", nx.x, nx.y);
                output.TexCoords[i] = nx;
            }
        }
    }

    /******************* 3. Mesh Simplification *****************/
    void SimplifyMesh(Engine::SurfaceMesh const & input, Engine::SurfaceMesh & output, float simplification_ratio) {

        DCEL G(input);
        if (! G.IsManifold()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::SimplifyMesh(..): Non-manifold mesh.");
            return;
        }
        // We only allow watertight mesh.
        if (! G.IsWatertight()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::SimplifyMesh(..): Non-watertight mesh.");
            return;
        }

        // Copy.
        output = input;

        // Compute Kp matrix of the face f.
        auto UpdateQ {
            [&G, &output] (DCEL::Triangle const * f) -> glm::mat4 {
                glm::mat4 Kp;
                // your code here:
                glm::vec3 u = output.Positions[f->VertexIndex(0)];
                glm::vec3 v = output.Positions[f->VertexIndex(1)];
                glm::vec3 w = output.Positions[f->VertexIndex(2)];
                glm::vec3 ex = { -1., -1., -1. };
                glm::vec3 n = glm::inverse(glm::transpose(glm::mat3(u, v, w))) * ex;
                double scale = sqrt((n.x * n.x + n.y * n.y + n.z * n.z));
                n /= scale;  // a ^ 2 + b ^ 2 + c ^ 2 + d = 1;
                glm::vec4 p = { n.x, n.y, n.z, 1. / scale };
                for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) Kp[i][j] = p[i] * p[j];
                return Kp;
            }
        };

        // The struct to record contraction info.
        struct ContractionPair {
            DCEL::HalfEdge const * edge;            // which edge to contract; if $edge == nullptr$, it means this pair is no longer valid
            glm::vec4              targetPosition;  // the targetPosition $v$ for vertex $edge->From()$ to move to
            float                  cost;            // the cost $v.T * Qbar * v$
        };

        // Given an edge (v1->v2), the positions of its two endpoints (p1, p2) and the Q matrix (Q1+Q2),
        //     return the ContractionPair struct.
        static constexpr auto MakePair {
            [] (DCEL::HalfEdge const * edge,
                glm::vec3 const & p1,
                glm::vec3 const & p2,
                glm::mat4 const & Q
            ) -> ContractionPair {
                // your code here:
                glm::mat4 tQ = glm::transpose(Q);
                tQ[0][3] = tQ[1][3] = tQ[2][3] = 0; tQ[3][3] = 1;
                glm::vec4 b = { 0, 0, 0, 1 };
                glm::vec4 v = {(p1 + p2) / 2.f, 1};
                if(glm::determinant(tQ) >= 0.001) v = glm::inverse(tQ) * b;
                float cost = glm::dot(v, Q * v);
                return { edge, v, cost };
            }
        };

        // pair_map: map EdgeIdx to index of $pairs$
        // pairs:    store ContractionPair
        // Qv:       $Qv[idx]$ is the Q matrix of vertex with index $idx$
        // Kf:       $Kf[idx]$ is the Kp matrix of face with index $idx$
        std::unordered_map<DCEL::EdgeIdx, std::size_t> pair_map; 
        std::vector<ContractionPair>                  pairs; 
        std::vector<glm::mat4>                         Qv(G.NumOfVertices(), glm::mat4(0));
        std::vector<glm::mat4>                         Kf(G.NumOfFaces(),    glm::mat4(0));

        // Initially, we compute Q matrix for each faces and it accumulates at each vertex.
        for (auto f : G.Faces()) {
            auto Q                 = UpdateQ(f);
            Qv[f->VertexIndex(0)] += Q;
            Qv[f->VertexIndex(1)] += Q;
            Qv[f->VertexIndex(2)] += Q;
            Kf[G.IndexOf(f)]       = Q;
        }

        pair_map.reserve(G.NumOfFaces() * 3);
        pairs.reserve(G.NumOfFaces() * 3 / 2);

        // Initially, we make pairs from all the contractable edges.
        for (auto e : G.Edges()) {
            if (! G.IsContractable(e)) continue;
            auto v1                            = e->From();
            auto v2                            = e->To();
            auto pair                          = MakePair(e, input.Positions[v1], input.Positions[v2], Qv[v1] + Qv[v2]);
            pair_map[G.IndexOf(e)]             = pairs.size();
            pair_map[G.IndexOf(e->TwinEdge())] = pairs.size();
            pairs.emplace_back(pair);
        }

        // Loop until the number of vertices is less than $simplification_ratio * initial_size$.
        while (G.NumOfVertices() > simplification_ratio * Qv.size()) {
            // Find the contractable pair with minimal cost.
            std::size_t min_idx = ~0;
            for (std::size_t i = 1; i < pairs.size(); ++i) {
                if (! pairs[i].edge) continue;
                if (!~min_idx || pairs[i].cost < pairs[min_idx].cost) {
                    if (G.IsContractable(pairs[i].edge)) min_idx = i;
                    else pairs[i].edge = nullptr;
                }
            }
            if (!~min_idx) break;

            // top:    the contractable pair with minimal cost
            // v1:     the reserved vertex
            // v2:     the removed vertex
            // result: the contract result
            // ring:   the edge ring of vertex v1
            ContractionPair & top    = pairs[min_idx];
            auto               v1     = top.edge->From();
            auto               v2     = top.edge->To();
            auto               result = G.Contract(top.edge);
            auto               ring   = G.Vertex(v1)->Ring();

            top.edge             = nullptr;            // The contraction has already been done, so the pair is no longer valid. Mark it as invalid.
            output.Positions[v1] = top.targetPosition; // Update the positions.

            // We do something to repair $pair_map$ and $pairs$ because some edges and vertices no longer exist.
            for (int i = 0; i < 2; ++i) {
                DCEL::EdgeIdx removed           = G.IndexOf(result.removed_edges[i].first);
                DCEL::EdgeIdx collapsed         = G.IndexOf(result.collapsed_edges[i].second);
                pairs[pair_map[removed]].edge   = result.collapsed_edges[i].first;
                pairs[pair_map[collapsed]].edge = nullptr;
                pair_map[collapsed]             = pair_map[G.IndexOf(result.collapsed_edges[i].first)];
            }

            // For the two wing vertices, each of them lose one incident face.
            // So, we update the Q matrix.
            Qv[result.removed_faces[0].first] -= Kf[G.IndexOf(result.removed_faces[0].second)];
            Qv[result.removed_faces[1].first] -= Kf[G.IndexOf(result.removed_faces[1].second)];

            // For the vertex v1, Q matrix should be recomputed.
            // And as the position of v1 changed, all the vertices which are on the ring of v1 should update their Q matrix as well.
            Qv[v1] = glm::mat4(0);
            for (auto e : ring) {
                // your code here:
                //     1. Compute the new Kp matrix for $e->Face()$.
                //     2. According to the difference between the old Kp (in $Kf$) and the new Kp (computed in step 1),
                //        update Q matrix of each vertex on the ring (update $Qv$).
                //     3. Update Q matrix of vertex v1 as well (update $Qv$).
                //     4. Update $Kf$.
                auto f                 = e->Face();
                auto Q                 = UpdateQ(f);
                Qv[e->From()] += Q - Kf[G.IndexOf(f)];
                Qv[e->To()] += Q - Kf[G.IndexOf(f)];
                Qv[v1] += Q;
                Kf[G.IndexOf(f)] = Q;
            }

            // Finally, as the Q matrix changed, we should update the relative $ContractionPair$ in $pairs$.
            // Any pair with the Q matrix of its endpoints changed, should be remade by $MakePair$.
            // your code here:
            for (auto e1 : ring) {
                auto vv1 = e1->From();
                auto tring   = G.Vertex(vv1)->Ring();
                for (auto e2 : tring){
                    if (!G.IsContractable(e2->NextEdge())){
                        pairs[pair_map[G.IndexOf(e2->NextEdge())]].edge=nullptr;
                    }
                    else{
                        auto vv2 = e2->To();
                        auto tpair = MakePair(e2->NextEdge(), output.Positions[vv1], output.Positions[vv2], Qv[vv1] + Qv[vv2]);
                        pairs[pair_map[G.IndexOf(e2->NextEdge())]].targetPosition = tpair.targetPosition;
                        pairs[pair_map[G.IndexOf(e2->NextEdge())]].cost = tpair.cost;
                    }
                }
            }
        }

        // In the end, we check if the result mesh is watertight and manifold.
        if (! G.DebugWatertightManifold()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::SimplifyMesh(..): Result is not watertight manifold.");
        }

        auto exported = G.ExportMesh();
        output.Indices.swap(exported.Indices);
    }

    /******************* 4. Mesh Smoothing *****************/
    void SmoothMesh(Engine::SurfaceMesh const & input, Engine::SurfaceMesh & output, std::uint32_t numIterations, float lambda, bool useUniformWeight) {
        // Define function to compute cotangent value of the angle v1-vAngle-v2
        static constexpr auto GetCotangent {
            [] (glm::vec3 vAngle, glm::vec3 v1, glm::vec3 v2) -> float {
                // your code here:
                glm::vec3 a = v1 - vAngle;
                glm::vec3 b = v2 - vAngle;
                float l1 = sqrt(glm::dot(a, a)), l2 = sqrt(glm::dot(b, b));
                float vcos = glm::dot(a, b) / l1, vsin = sqrt(1 - vcos * vcos);
                return fabs(vcos / vsin);
            }
        };

        DCEL G(input);
        if (! G.IsManifold()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::SmoothMesh(..): Non-manifold mesh.");
            return;
        }
        // We only allow watertight mesh.
        if (! G.IsWatertight()) {
            spdlog::warn("VCX::Labs::GeometryProcessing::SmoothMesh(..): Non-watertight mesh.");
            return;
        }

        Engine::SurfaceMesh prev_mesh;
        prev_mesh.Positions = input.Positions;
        for (std::uint32_t iter = 0; iter < numIterations; ++iter) {
            Engine::SurfaceMesh curr_mesh = prev_mesh;
            for (std::size_t i = 0; i < input.Positions.size(); ++i) {
                // your code here: curr_mesh.Positions[i] = ...
                glm::vec3 newpos(0);
                float tot = 0;
                auto edges = G.Vertex(i)->Ring();
                for(auto e : edges) {
                    auto t = e->PrevEdge();
                    int x = t->To(), y = e->To(), z = t->TwinEdge()->NextEdge()->To();
                    if(useUniformWeight) {
                        tot += 1;
                        newpos += input.Positions[x];
                    }
                    else {
                        float ret = GetCotangent(input.Positions[i], input.Positions[x], input.Positions[y]) +
                            GetCotangent(input.Positions[i], input.Positions[x], input.Positions[z]);
                        newpos += ret * input.Positions[x];
                        tot += ret;
                    }
                }
                curr_mesh.Positions[i] = lambda * (newpos / tot) + input.Positions[i] * (1 - lambda);
            }
            // Move curr_mesh to prev_mesh.
            prev_mesh.Swap(curr_mesh);
        }
        // Move prev_mesh to output.
        output.Swap(prev_mesh);
        // Copy indices from input.
        output.Indices = input.Indices;
    }

    /******************* 5. Marching Cubes *****************/
    void MarchingCubes(Engine::SurfaceMesh & output, const std::function<float(const glm::vec3 &)> & sdf, const glm::vec3 & grid_min, const float dx, const int n) {
        // your code here:
        auto getNodePos = [&](glm::vec3 t, int i) {
            t.x += (i & 1) * dx;
            t.y += (i >> 1 & 1) * dx;
            t.z += (i >> 2 & 1) * dx;
            return t;
        };
        auto unit = [&](int i) {
            if(i == 0) return glm::vec3(1, 0, 0);
            if(i == 1) return glm::vec3(0, 1, 0);
            if(i == 2) return glm::vec3(0, 0, 1);
            return glm::vec3(0, 0, 0);
        };
        auto getEdgePos = [&](glm::vec3 t, int j) {
            return t + dx * (j & 1) * unit(((j >> 2) + 1) % 3) + dx * ((j >> 1) & 1) * unit(((j >> 2) + 2) % 3);
        };
        auto getNormal = [&](glm::vec3 pos) { 
            float l = 1e-2;
            float fpos = sdf(pos), fx = sdf(pos + unit(0) * l), fy = sdf(pos + unit(1) * l), fz = sdf(pos + unit(2) * l);
            fx -= fpos, fy -= fpos, fz -= fpos;
            fx /= l, fy /= l, fz /= l;
            return glm::normalize(glm::vec3(fx, fy, fz));
        };
        int pos[12];
        for(int x = 0; x < n; x++)
            for(int y = 0; y < n; y++)
                for(int z = 0; z < n; z++) {
                    int stu = 0;
                    glm::vec3 v0 = grid_min + glm::vec3(dx * x, dx * y, dx * z);
                    for(int s = 7; s >= 0; s--) stu = stu << 1 | (sdf(getNodePos(v0, s)) >= 0);
                    int edgestu = c_EdgeStateTable[stu];
                    for(int s = 0; s < 12; s++) if(edgestu >> s & 1) {
                        pos[s] = output.Positions.size();
                        glm::vec3   stpos = getEdgePos(v0, s),
                                    edpos = stpos + dx * unit(s >> 2);
                        float v1 = sdf(edpos), v2 = sdf(stpos);
                        glm::vec3   edgepos = ((v2) * edpos + (-v1) * stpos) / (v2 - v1);
                        output.Positions.push_back(edgepos);
                        glm::vec3   stn = getNormal(stpos), 
                                    edn = getNormal(edpos), 
                                    normal = ((v2) * edn + (-v1) * stn) / (v2 - v1) ;
                        output.Normals.push_back(normal);
                    }
                    for(int t = 0; t < 6; t++) if(c_EdgeOrdsTable[stu][t * 3] != -1) {
                        int a = c_EdgeOrdsTable[stu][t * 3], 
                            b = c_EdgeOrdsTable[stu][t * 3 + 1],
                            c = c_EdgeOrdsTable[stu][t * 3 + 2];
                        output.Indices.push_back(pos[c]);
                        output.Indices.push_back(pos[b]);
                        output.Indices.push_back(pos[a]);
                    }
                }
    }
} // namespace VCX::Labs::GeometryProcessing
