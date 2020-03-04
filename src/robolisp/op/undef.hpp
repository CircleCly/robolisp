#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class Undef : public Op
{
  public :
    
    explicit Undef(Env *env); 
    
  private :
    
    Env *env_;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Sym &&sym) override;
};

OpPtr create_undef(Env *env);

}
// namespace op

}
// namespace robolisp::impl
