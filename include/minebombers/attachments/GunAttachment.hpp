#pragma once

#include <string>
#include "minebombers/data/GunParameters.hpp"

class GunAttachment : public NodeAttachment {
public:
  GunAttachment(std::shared_ptr<GunParameters> param): _param(param){}

  auto parameters() const -> std::shared_ptr<GunParameters> {
    return _param;
  }

private:
  std::shared_ptr<GunParameters> _param;
};
