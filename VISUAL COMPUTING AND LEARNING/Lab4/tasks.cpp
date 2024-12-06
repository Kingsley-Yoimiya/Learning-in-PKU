#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <spdlog/spdlog.h>
#include <iostream>
#include "Labs/4-Animation/tasks.h"
#include "IKSystem.h"
#include "CustomFunc.inl"


namespace VCX::Labs::Animation {
    void ForwardKinematics(IKSystem & ik, int StartIndex) {
        if (StartIndex == 0) {
            ik.JointGlobalRotation[0] = ik.JointLocalRotation[0];
            ik.JointGlobalPosition[0] = ik.JointLocalOffset[0];
            StartIndex                = 1;
        }
        
        for (int i = StartIndex; i < ik.JointLocalOffset.size(); i++) {
            // your code here: forward kinematics, update JointGlobalPosition and JointGlobalRotation
            ik.JointGlobalRotation[i] = ik.JointGlobalRotation[i - 1] * ik.JointLocalRotation[i];
            auto transRotation = glm::mat4_cast(ik.JointGlobalRotation[i - 1]) * glm::vec4(ik.JointLocalOffset[i], 1.0f);
            ik.JointGlobalPosition[i] = ik.JointGlobalPosition[i - 1] + glm::vec3(transRotation.x, transRotation.y, transRotation.z) / transRotation.w;
        }
    }

    void InverseKinematicsCCD(IKSystem & ik, const glm::vec3 & EndPosition, int maxCCDIKIteration, float eps) {
        ForwardKinematics(ik, 0);
        // These functions will be useful: glm::normalize, glm::rotation, glm::quat * glm::quat
        for (int CCDIKIteration = 0; CCDIKIteration < maxCCDIKIteration && glm::l2Norm(ik.EndEffectorPosition() - EndPosition) > eps; CCDIKIteration++) {
            // your code here: ccd ik
            for (int i = ik.NumJoints() - 2; i >= 0; i--) {
                ik.JointLocalRotation[i] = 
                    glm::rotation(
                        glm::normalize(ik.EndEffectorPosition() - ik.JointGlobalPosition[i]), 
                        glm::normalize(EndPosition              - ik.JointGlobalPosition[i])
                    ) * ik.JointLocalRotation[i];                
                ForwardKinematics(ik, i);
            }
        }
    }

    void InverseKinematicsFABR(IKSystem & ik, const glm::vec3 & EndPosition, int maxFABRIKIteration, float eps) {
        ForwardKinematics(ik, 0);
        int nJoints = ik.NumJoints();
        std::vector<glm::vec3> backward_positions(nJoints, glm::vec3(0, 0, 0)), forward_positions(nJoints, glm::vec3(0, 0, 0));
        for (int IKIteration = 0; IKIteration < maxFABRIKIteration && glm::l2Norm(ik.EndEffectorPosition() - EndPosition) > eps; IKIteration++) {
            // task: fabr ik
            // backward update
            glm::vec3 next_position         = EndPosition;
            backward_positions[nJoints - 1] = EndPosition;

            for (int i = nJoints - 2; i >= 0; i--) {
                // your code here
                auto r = glm::normalize(ik.JointGlobalPosition[i] - next_position);
                next_position = next_position + r * ik.JointOffsetLength[i + 1];
                backward_positions[i] = next_position; 
            }

            // forward update
            glm::vec3 now_position = ik.JointGlobalPosition[0];
            forward_positions[0] = ik.JointGlobalPosition[0];
            for (int i = 0; i < nJoints - 1; i++) {
                // your code here
                auto r =  glm::normalize(backward_positions[i + 1] - now_position);
                now_position = now_position + r * ik.JointOffsetLength[i + 1];
                forward_positions[i + 1] = now_position;
            }
            ik.JointGlobalPosition = forward_positions; // copy forward positions to joint_positions
        }

        // Compute joint rotation by position here.
        for (int i = 0; i < nJoints - 1; i++) {
            ik.JointGlobalRotation[i] = glm::rotation(glm::normalize(ik.JointLocalOffset[i + 1]), glm::normalize(ik.JointGlobalPosition[i + 1] - ik.JointGlobalPosition[i]));
        }
        ik.JointLocalRotation[0] = ik.JointGlobalRotation[0];
        for (int i = 1; i < nJoints - 1; i++) {
            ik.JointLocalRotation[i] = glm::inverse(ik.JointGlobalRotation[i - 1]) * ik.JointGlobalRotation[i];
        }
        ForwardKinematics(ik, 0);
    }

    IKSystem::Vec3ArrPtr IKSystem::BuildCustomTargetPosition() {
        // get function from https://www.wolframalpha.com/input/?i=Albert+Einstein+curve
        // int nums = 5000;
        // using Vec3Arr = std::vector<glm::vec3>;
        // std::shared_ptr<Vec3Arr> custom(new Vec3Arr(nums));
        // int index = 0;
        // for (int i = 0; i < nums; i++) {
        //     float x_val = 1.5e-3f * custom_x(92 * glm::pi<float>() * i / nums);
        //     float y_val = 1.5e-3f * custom_y(92 * glm::pi<float>() * i / nums);
        //     if (std::abs(x_val) < 1e-3 || std::abs(y_val) < 1e-3) continue;
        //     (*custom)[index++] = glm::vec3(1.6f - x_val, 0.0f, y_val - 0.2f);
        // }
        // custom->resize(index);
        // return custom;
        std::string C = R"(
  ...   ...   ...   ..........
   ... ...    ...   ...    ...
    ....      ...   ......... 
     ...      ...   ... ...   
     ...  ... ...   ...  ...  
     ...    ....    ...   ... 
)";
        using Vec3Arr = std::vector<glm::vec3>;
        Vec3Arr custom;
        for (int i = 0; i < 6; i++) for(int j = 0; j < 30; j++) if(C[i * 31 + j] == '.')
            custom.emplace_back(glm::vec3(i / 6.f, 0.f, j / 30.f));
        std::shared_ptr<Vec3Arr> custom_ptr(new Vec3Arr(custom.size()));
        std::copy(custom.begin(), custom.end(), custom_ptr->begin());
        return custom_ptr;
    }

    static Eigen::VectorXf glm2eigen(std::vector<glm::vec3> const & glm_v) {
        Eigen::VectorXf v = Eigen::Map<Eigen::VectorXf const, Eigen::Aligned>(reinterpret_cast<float const *>(glm_v.data()), static_cast<int>(glm_v.size() * 3));
        return v;
    }

    static std::vector<glm::vec3> eigen2glm(Eigen::VectorXf const & eigen_v) {
        return std::vector<glm::vec3>(
            reinterpret_cast<glm::vec3 const *>(eigen_v.data()),
            reinterpret_cast<glm::vec3 const *>(eigen_v.data() + eigen_v.size())
        );
    }

    static Eigen::SparseMatrix<float> CreateEigenSparseMatrix(std::size_t n, std::vector<Eigen::Triplet<float>> const & triplets) {
        Eigen::SparseMatrix<float> matLinearized(n, n);
        matLinearized.setFromTriplets(triplets.begin(), triplets.end());
        return matLinearized;
    }

    // solve Ax = b and return x
    static Eigen::VectorXf ComputeSimplicialLLT(
        Eigen::SparseMatrix<float> const & A,
        Eigen::VectorXf const & b) {
        auto solver = Eigen::SimplicialLLT<Eigen::SparseMatrix<float>>(A);
        return solver.solve(b);
    }

    Eigen::VectorXf calc_y(MassSpringSystem &system, Eigen::VectorXf const Positions, float const dt) {
        return  Positions + dt * glm2eigen(system.Velocities) + 
                dt * dt / system.Mass * glm2eigen(
                    std::vector<glm::vec3>(
                        system.Positions.size(), 
                        glm::vec3(0, -system.Gravity, 0)
                    )
                );
    }

    glm::vec3 getVec3(Eigen::VectorXf x, int pos) { 
        return glm::vec3({ x[pos * 3], x[pos * 3 + 1], x[pos * 3 + 2] });
    }

    float calc_g(MassSpringSystem &system, Eigen::VectorXf const Positions, Eigen::VectorXf const y, float const dt) {
        Eigen::VectorXf tmp = Positions - y;
        float ret = tmp.dot(tmp) * system.Mass / (2 * dt * dt);
        for(const auto spring : system.Springs) {
            auto const p0 = spring.AdjIdx.first;
            auto const p1 = spring.AdjIdx.second;
            glm::vec3 const x01 = getVec3(Positions, p1) - getVec3(Positions, p0);
            // glm::vec3 const e01 = glm::normalize(x01);
            ret += 0.5 * system.Stiffness * glm::pow(glm::length(x01) - spring.RestLength, 2);
        }
        return ret;
    }
    
    Eigen::VectorXf calc_dg(MassSpringSystem &system, Eigen::VectorXf const Positions, Eigen::VectorXf const y, float const dt) {
        Eigen::VectorXf ret = (Positions - y) * system.Mass / (dt * dt); //Eigen::VectorXf::Zero(Positions.size());
        for(const auto spring : system.Springs) {
            auto const p0 = spring.AdjIdx.first;
            auto const p1 = spring.AdjIdx.second;
            glm::vec3 const x01 = getVec3(Positions, p1) - getVec3(Positions, p0);
            glm::vec3 const e01 = glm::normalize(x01);
            glm::vec3 f = system.Stiffness * (glm::length(x01) - spring.RestLength) * e01;
            for(int i = 0; i < 3; i++) {
                ret[p0 * 3 + i] -= f[i];
                ret[p1 * 3 + i] += f[i];
            }
        }
        return ret;
    }

    Eigen::SparseMatrix<float> calc_ddg(MassSpringSystem &system, Eigen::VectorXf const Positions, Eigen::VectorXf const y, float const dt) {
        std::vector<Eigen::Triplet<float>> triplets;
        for(const auto spring : system.Springs) {
            auto const p0 = spring.AdjIdx.first;
            auto const p1 = spring.AdjIdx.second;
            glm::vec3 const x01 = getVec3(Positions, p1) - getVec3(Positions, p0);
            glm::vec3 const e01 = glm::normalize(x01);
            glm::mat3 f(0);
            for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) 
                f[i][j] += system.Stiffness * (
                    x01[i] * x01[j] / glm::dot(x01, x01) + 
                    (1 - spring.RestLength / glm::length(x01)) * ((i == j) - x01[i] * x01[j] / glm::dot(x01, x01))
                );
            for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) {
                triplets.emplace_back(p0 * 3 + i, p0 * 3 + j, f[i][j]);
                triplets.emplace_back(p0 * 3 + i, p1 * 3 + j, -f[i][j]);
                triplets.emplace_back(p1 * 3 + i, p0 * 3 + j, -f[i][j]);
                triplets.emplace_back(p1 * 3 + i, p1 * 3 + j, +f[i][j]);
            }
        }
        for(int i = 0; i < Positions.size() * 3; i++) {
            triplets.emplace_back(i, i, system.Mass / (dt * dt));
        }
        return CreateEigenSparseMatrix(Positions.size() * 3, triplets);
    }

    void AdvanceMassSpringSystem(MassSpringSystem & system, float const dt) {
        Eigen::VectorXf x_origin = glm2eigen(system.Positions);
        Eigen::VectorXf y = calc_y(system, glm2eigen(system.Positions), dt);
        Eigen::VectorXf x = y;
        float g = calc_g(system, x, y, dt);
        int numIter = 5;
        for(int k = 1; k < numIter; k++) {
            Eigen::VectorXf delta_g = calc_dg(system, x, y, dt);
            Eigen::SparseMatrix delta_g2 = calc_ddg(system, glm2eigen(system.Positions), y, dt);
            Eigen::VectorXf delta_x = ComputeSimplicialLLT(delta_g2, -delta_g);
            float beta = 0.8, alpha = 1 / beta, g_new = g, gamma = 0.001;
            Eigen::VectorXf x_new = x; //, y_new = y;
            do {
                alpha *= beta;
                x_new = x + alpha * delta_x;
                // y_new = calc_y(system, x_new, dt); 
                g_new = calc_g(system, x_new, y, dt);
            } while(g_new > g + alpha * gamma * delta_g.dot(delta_x));
            x = x_new;
            g = g_new;
            // y = y_new;
        }
        std::vector<glm::vec3> newV = eigen2glm((x - x_origin) / dt);
        std::vector<glm::vec3> newX = eigen2glm(x);
        for(int i = 0; i < system.Positions.size(); i++) {
            if(system.Fixed[i]) continue;
            system.Positions[i] = newX[i];
            system.Velocities[i] = newV[i];
        }
    }
}
