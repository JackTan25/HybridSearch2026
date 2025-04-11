

import warnings
from hybridsearch.common import hybridsearchException
from hybridsearch.errors import ErrorCode


def binary_exp_to_paser_exp(binary_expr_key) -> str:
    if binary_expr_key == "eq":
        return "="
    elif binary_expr_key == "gt":
        return ">"
    elif binary_expr_key == "lt":
        return "<"
    elif binary_expr_key == "gte":
        return ">="
    elif binary_expr_key == "lte":
        return "<="
    elif binary_expr_key == "neq":
        return "!="
    elif binary_expr_key == "and":
        return "and"
    elif binary_expr_key == "or":
        return "or"
    elif binary_expr_key == "add":
        return "+"
    elif binary_expr_key == "sub":
        return "-"
    elif binary_expr_key == "mul":
        return "*"
    elif binary_expr_key == "div":
        return "/"
    elif binary_expr_key == "mod":
        return "%"
    else:
        raise hybridsearchException(ErrorCode.INVALID_EXPRESSION, f"unknown binary expression: {binary_expr_key}")


def deprecated_api(message):
    warnings.warn(message, DeprecationWarning, stacklevel=2)
