#include "add.hpp"

#include <cinttypes>
#include <cstddef>
#include <string>
#include <utility>
#include <variant>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"
#include "../val/float_num.hpp"
#include "../val/str.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Add::take_res()
{
    ValPtr res_val = nullptr;
    if (std::holds_alternative<ValPtrList>(res_))
        res_val = val::create_list(std::move(std::get<ValPtrList>(res_)));
    else if (std::holds_alternative<int32_t>(res_))
        res_val = val::create_int_num(std::get<int32_t>(res_));
    else if (std::holds_alternative<float>(res_))
        res_val = val::create_float_num(std::get<float>(res_));
    else if (std::holds_alternative<std::string>(res_))
        res_val = val::create_str(std::get<std::string>(res_));
    
    return res_val;
}

std::string Add::get_desc() const
{
    return "add";
}

std::size_t Add::get_min_args() const
{
    return 2;
}

std::size_t Add::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Add::process(val::List &&list)
{
    if (get_arg_cnt() == 1)
    {
        res_ = list.take();
    }
    else if (std::holds_alternative<ValPtrList>(res_))
    {
        ValPtrList res_val_list = {};
        if(::call(create_add,
                  std::get<ValPtrList>(res_),
                  list.get(),
                  nullptr,
                  [this, &res_val_list](ValPtr val, bool is_err)
                  {
                      if (is_err)
                          reg_err(std::move(val));
                      else
                          res_val_list.push_back(std::move(val));
                  })) // lists have different lengths
        {
            reg_err(val::Err::LOGIC_ERR,
                    "cannot operate on lists with different lengths: '" + val::to_str(std::get<ValPtrList>(res_))
                    + "' and '" + list.to_str() + "'");
        }
        else // lists have the same length
        {
            res_ = std::move(res_val_list);
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Add::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
        res_ = int_num.get();
    else if (std::holds_alternative<int32_t>(res_))
        std::get<int32_t>(res_) += int_num.get();
    else if (std::holds_alternative<float>(res_))
        std::get<float>(res_) += int_num.get();
    else
        reg_illegal_arg(int_num);
}

void Add::process(val::FloatNum &&float_num)
{
    if (get_arg_cnt() == 1)
        res_ = float_num.get();
    else if (std::holds_alternative<float>(res_))
        std::get<float>(res_) += float_num.get();
    else if (std::holds_alternative<int32_t>(res_))
        res_ = std::get<int32_t>(res_) + float_num.get();
    else
        reg_illegal_arg(float_num);
}

void Add::process(val::Str &&str)
{
    if (get_arg_cnt() == 1)
        res_ = str.take();
    else if (std::holds_alternative<std::string>(res_))
        std::get<std::string>(res_) += str.get();
    else
        reg_illegal_arg(str);
}

OpPtr robolisp::impl::op::create_add(Env */*env*/)
{
    return OpPtr(new Add());
}
