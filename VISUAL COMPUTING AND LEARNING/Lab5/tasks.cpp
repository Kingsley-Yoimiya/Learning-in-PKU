#include "Labs/5-Visualization/tasks.h"

#include <numbers>

using VCX::Labs::Common::ImageRGB;
namespace VCX::Labs::Visualization {
    const float upperMargin = 0.1;
    const float lowerMargin = 0.9;
    const float leftMargin = 0.1;
    const float rightMargin = 0.9;
    const float delta = (rightMargin - leftMargin) / 6;
    const float checkWidth = delta / 3;
    const glm::vec2 rectSize(delta / 10, lowerMargin - upperMargin);
    const glm::vec4 unselectedColor(0.5, 0.5, 0.5, 0.5);
    const glm::vec4 selectedColor(0.866f, 0.627f, 0.866f, 0.5f);
    const glm::vec4 lightStartColor(0.275f, 0.51f, 0.705f, 0.9f);
    const glm::vec4 lightEndColor(0.647f, 0.165f, 0.165f, 0.9f);
    const glm::vec4 lineColor(0, 0, 0, 1);
    const glm::vec4 notInRangeColor(0.5, 0.5, 0.5, 0.07);
    struct CoordinateStates {
        // your code here
        bool painted;
        float range_in_canvas[7][2];
        float range_in_data[7][2];
        int selectedId;
        std::vector <std::array<float, 7>> num_data;
        std::string getCaption(int id) {
            switch (id)
            {
            case 0:
                return "cylinders";
            case 1:
                return "displacement";
            case 2:
                return "weight";
            case 3:
                return "horsepower";
            case 4:
                return "acceleration";
            case 5:
                return "mileage";
            case 6:
                return "year";
            default:
                return "fucked";
                break;
            }
        }
        std::string getUnitName(int id) {
            switch (id)
            {
            case 0:
                return "";
            case 1:
                return " sq in";
            case 2:
                return " lbs";
            case 3:
                return " hp";
            case 4:
                return " sec";
            case 5:
                return " mpg";
            case 6:
                return "";
            default:
                return "fucked";
                break;
            }
        }
        CoordinateStates(std::vector<Car> const & data) {
            // your code here
            painted = false;
            selectedId = -1;
            for(int i = 0; i < 7; i++) {
                range_in_data[i][0] = std::numeric_limits<float>::max();
                range_in_data[i][1] = std::numeric_limits<float>::min();
            }
            for(auto v : data) {
                num_data.push_back({static_cast<float>(v.cylinders), v.displacement, v.weight, v.horsepower, v.acceleration, v.mileage, static_cast<float>(v.year)});
                for(int i = 0; i < 7; i++) {
                    range_in_data[i][0] = std::min(range_in_data[i][0], num_data.back()[i]);
                    range_in_data[i][1] = std::max(range_in_data[i][1], num_data.back()[i]);
                }
            }
            for(int i = 0; i < 7; i++) range_in_canvas[i][0] = upperMargin, range_in_canvas[i][1] = lowerMargin;
        }
        int getAxis(glm::vec2 pos) {
            if(pos.y < upperMargin || pos.y > lowerMargin) return -1;
            for(int i = 0; i < 7; i++) { 
                float centerpos = delta * i + leftMargin;
                if(centerpos <= pos.x && pos.x <= centerpos + rectSize.x) return i;
            }
            return -1;
        }
        int getRangeAxis(glm::vec2 pos) {
            if(pos.y < upperMargin || pos.y > lowerMargin) return -1;
            for(int i = 0; i < 7; i++) { 
                float centerpos = delta * i + leftMargin;
                if(centerpos - checkWidth <= pos.x && pos.x <= centerpos + checkWidth) return i;
            }
            return -1;
        }
        bool Update(InteractProxy const & proxy) {
            if(proxy.IsHovering()) {
                int tmpAxis = getAxis(proxy.MousePos());
                if(tmpAxis != -1) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand); // set to resize
                else {
                    tmpAxis = getRangeAxis(proxy.MousePos());
                    if(tmpAxis != -1) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); // set to hand
                    else ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); // set to default
                }
            } else ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); // set to default
            bool need_upd = proxy.IsHovering() && (proxy.IsClicking() || proxy.IsDragging());
            if(!need_upd && painted) {
                return false;
            }
            painted = true;
            if(need_upd) {
                if(proxy.IsClicking()) {
                    int tmpAxis = getAxis(proxy.MousePos());
                    if(tmpAxis != -1) {
                        selectedId = tmpAxis; 
                        return true;
                    } 
                    tmpAxis = getRangeAxis(proxy.MousePos());
                    if(tmpAxis == -1) return false;
                    /* reset range */
                    range_in_canvas[tmpAxis][0] = upperMargin;
                    range_in_canvas[tmpAxis][1] = lowerMargin;
                    selectedId = tmpAxis;
                    return true;
                } else { // Dragging
                    int tmpAxis = getAxis(proxy.DraggingStartPoint());
                    if(tmpAxis != -1) { // start drag the range box.
                        float rangeDelta = (proxy.MouseDeltaPos()).y / 2;
                        rangeDelta = std::min(rangeDelta, lowerMargin - range_in_canvas[tmpAxis][1]);
                        rangeDelta = std::max(rangeDelta, upperMargin - range_in_canvas[tmpAxis][0]);
                        range_in_canvas[tmpAxis][0] += rangeDelta;
                        range_in_canvas[tmpAxis][1] += rangeDelta;
                        selectedId = tmpAxis;
                        return true;
                    } 
                    tmpAxis = getRangeAxis(proxy.DraggingStartPoint());
                    if(tmpAxis == -1) return false;
                    // start selected the range box.
                    selectedId = tmpAxis;
                    float rangeLow = proxy.DraggingStartPoint().y;
                    float rangeHigh = proxy.MousePos().y;
                    if(rangeLow > rangeHigh) std::swap(rangeLow, rangeHigh);
                    range_in_canvas[tmpAxis][0] = std::max(rangeLow, upperMargin);
                    range_in_canvas[tmpAxis][1] = std::min(rangeHigh, lowerMargin);
                    return true;
                }
            }
            return true;
        }
        void Paint(Common::ImageRGB & input) {
            SetBackGround(input, glm::vec4(1, 1, 1, 0));

            /* Paint Lines */
            int checkAxis = selectedId == -1 ? 0 : selectedId;
            for(auto v : num_data) { 
                bool ok = true;
                for(int i = 0; i < 7 && ok; i++) {
                    float centerpos = delta * i + leftMargin;
                    float pos = (v[i] - range_in_data[i][0]) / (range_in_data[i][1] - range_in_data[i][0]) * (lowerMargin - upperMargin) + upperMargin;
                    if(pos > range_in_canvas[i][1] || pos < range_in_canvas[i][0]) ok = false;
                }
                glm::vec2 lastpos;
                float rate = (v[checkAxis] - range_in_data[checkAxis][0]) / (range_in_data[checkAxis][1] - range_in_data[checkAxis][0]);
                glm::vec4 color = rate * lightStartColor + (1 - rate) * lightEndColor;
                if(!ok) color = notInRangeColor;
                for(int i = 0; i < 7; i++) {
                    float centerpos = delta * i + leftMargin;
                    glm::vec2 pos(centerpos + rectSize.x / 2, (v[i] - range_in_data[i][0]) / (range_in_data[i][1] - range_in_data[i][0]) * (lowerMargin - upperMargin) + upperMargin);
                    if(i != 0) {
                        DrawLine(input, color, lastpos, pos, 0.00002);
                    }
                    lastpos = pos;
                }
            }

            /* Paint boxs */
            for(int i = 0; i < 7; i++) { 
                float centerpos = delta * i + leftMargin;
                glm::vec4 color = unselectedColor;
                if(i == selectedId) {
                    float rate = (range_in_canvas[i][0] + range_in_canvas[i][1] - 2 * upperMargin) / (2 * lowerMargin - 2 * upperMargin);
                    color = rate * lightStartColor + (1 - rate) * lightEndColor;
                    color.w = 1;
                }
                DrawFilledRect(input, color, glm::vec2(centerpos, range_in_canvas[i][0]), glm::vec2(rectSize.x, range_in_canvas[i][1] - range_in_canvas[i][0]));
                DrawLine(input, lineColor, glm::vec2(centerpos + rectSize.x / 2, upperMargin), glm::vec2(centerpos + rectSize.x / 2, lowerMargin), 0.005);
                PrintText(input, lineColor, glm::vec2(centerpos, upperMargin / 3), upperMargin / 4, getCaption(i));
                int st = (range_in_canvas[i][0] - upperMargin) / (lowerMargin - upperMargin) * (range_in_data[i][1] - range_in_data[i][0]) + range_in_data[i][0];
                int ed = (range_in_canvas[i][1] - upperMargin) / (lowerMargin - upperMargin) * (range_in_data[i][1] - range_in_data[i][0]) + range_in_data[i][0];
                PrintText(input, lineColor, glm::vec2(centerpos, range_in_canvas[i][0] - upperMargin / 4), upperMargin / 4, std::to_string(st) +getUnitName(i));
                PrintText(input, lineColor, glm::vec2(centerpos, range_in_canvas[i][1] + upperMargin / 4), upperMargin / 4, std::to_string(ed) +getUnitName(i));
            }
        }
    };

    bool PaintParallelCoordinates(Common::ImageRGB & input, InteractProxy const & proxy, std::vector<Car> const & data, bool force) {
        static CoordinateStates states(data); // initialize
        bool change = states.Update(proxy); // update according to user input
        if (! force && ! change) return false; // determine to skip repainting
        states.Update(proxy);
        states.Paint(input); // visualize
        return true;
    }

    const float eps = 1e-7;

    void LIC(ImageRGB & output, Common::ImageRGB const & noise, VectorField2D const & field, int const & step) {
        // your code here
        int m = field.size.first, n = field.size.second;
        float t = 0;
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) {
            float y = i, x = j;
            glm::vec3 forward_sum(0, 0, 0);
            float forward_total = 0;
            for (int k = 0; k < step; k++) {
                float dx = field.At(int(y), int(x)).x, dy = field.At(int(y), int(x)).y;
                float dt_x = 0, dt_y = 0, dt = 0;
                if (dy > 0) dt_y = (floor(y) + 1 - y) / dy;
                else if (dy < 0) dt_y = (y - (ceil(y) - 1)) / (-dy);
                if (dx > 0) dt_x = (floor(x) + 1 - x) / dx;
                else if (dx < 0) dt_x = (x - (ceil(x) - 1)) / (-dx);
                if (abs(dx) < eps && abs(dy) < eps) dt = 0;
                else dt = std::min(dt_x, dt_y);
                x = std::min(std::max(x + dx * dt, 0.f), n - 1.f);
                y = std::min(std::max(y + dy * dt, 0.f), m - 1.f);
                float weight = pow(cos(t + 0.46 * k), 2);
                forward_sum += noise.At(int(y), int(x)) * weight;
                forward_total += weight;
            }
            y = i, x = j;
            glm::vec3 backward_sum(0, 0, 0);
            float backward_total = 0;
            for (int k = 1; k < step; k++) {
                float dx = field.At(int(y), int(x)).x, dy = field.At(int(y), int(x)).y;
                dy *=- 1, dx *= -1;
                float dt_x = 0, dt_y = 0, dt = 0;
                if (dy > 0) dt_y = (floor(y) + 1 - y) / dy;
                else if (dy < 0) dt_y = (y - (ceil(y) - 1)) / (-dy);
                if (dx > 0) dt_x = (floor(x) + 1 - x) / dx;
                else if (dx < 0) dt_x = (x - (ceil(x) - 1)) / (-dx);
                if (abs(dx) < eps && abs(dy) < eps) dt = 0;
                else dt = std::min(dt_x, dt_y);
                x = std::min(std::max(x + dx * dt, 0.f), n - 1.f);
                y = std::min(std::max(y + dy * dt, 0.f), m - 1.f);
                float weight = pow(cos(t - 0.46 * k), 2);
                forward_sum += noise.At(int(y), int(x)) * weight;
                forward_total += weight;
            }
            output.At(i, j) = (forward_sum + backward_sum) / (forward_total + backward_total);
        }
    }
}; // namespace VCX::Labs::Visualization