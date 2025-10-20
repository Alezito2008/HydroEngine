#pragma once

#include "Scene.h"

#include <memory>

namespace TreePanel {
    void Render(std::vector<std::unique_ptr<Scene>>& scenes);
}
