#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

namespace robolisp::impl
{

namespace op
{

class Cons : public Op
{
  private :
    
    ValPtrList res_val_list_ = {};
    ValPtr val_ = nullptr;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void default_process(Val &&val) override;
};

OpPtr create_cons(Env *env);

}
// namespace op

}
// namespace robolisp::impl
