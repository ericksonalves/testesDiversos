############################################
# Copyright (c) 2012 Microsoft Corporation
# 
# Z3 Python interface
#
# Author: Leonardo de Moura (leonardo)
############################################
import sys, io, z3
from z3consts import *
from z3core import *

##############################
#
# Configuration
#
##############################

# Z3 operator names to Z3Py
_z3_op_to_str = {
    Z3_OP_TRUE : 'True', Z3_OP_FALSE : 'False', Z3_OP_EQ : '==', Z3_OP_DISTINCT : 'Distinct', 
    Z3_OP_ITE : 'If', Z3_OP_AND : 'And', Z3_OP_OR : 'Or', Z3_OP_IFF : '==', Z3_OP_XOR : 'Xor',
    Z3_OP_NOT : 'Not', Z3_OP_IMPLIES : 'Implies', Z3_OP_IDIV : '/', Z3_OP_MOD : '%',
    Z3_OP_TO_REAL : 'ToReal', Z3_OP_TO_INT : 'ToInt', Z3_OP_POWER : '**', Z3_OP_IS_INT : 'IsInt', 
    Z3_OP_BADD : '+', Z3_OP_BSUB : '-', Z3_OP_BMUL : '*', Z3_OP_BOR : '|', Z3_OP_BAND : '&',
    Z3_OP_BNOT : '~', Z3_OP_BXOR : '^', Z3_OP_BNEG : '-', Z3_OP_BUDIV : 'UDiv', Z3_OP_BSDIV : '/', Z3_OP_BSMOD : '%',
    Z3_OP_BSREM : 'SRem', Z3_OP_BUREM : 'URem', Z3_OP_EXT_ROTATE_LEFT : 'RotateLeft', Z3_OP_EXT_ROTATE_RIGHT : 'RotateRight',
    Z3_OP_SLEQ : '<=', Z3_OP_SLT : '<', Z3_OP_SGEQ : '>=', Z3_OP_SGT : '>',
    Z3_OP_ULEQ : 'ULE', Z3_OP_ULT : 'ULT', Z3_OP_UGEQ : 'UGE', Z3_OP_UGT : 'UGT',
    Z3_OP_SIGN_EXT : 'SignExt', Z3_OP_ZERO_EXT : 'ZeroExt', Z3_OP_REPEAT : 'RepeatBitVec', 
    Z3_OP_BASHR : '>>', Z3_OP_BSHL : '<<', Z3_OP_BLSHR : 'LShR', 
    Z3_OP_CONCAT : 'Concat', Z3_OP_EXTRACT : 'Extract', Z3_OP_BV2INT : 'BV2Int',
    Z3_OP_ARRAY_MAP : 'Map', Z3_OP_SELECT : 'Select', Z3_OP_STORE : 'Store', 
    Z3_OP_CONST_ARRAY : 'K' 
    }

# List of infix operators
_z3_infix = [ 
    Z3_OP_EQ, Z3_OP_IFF, Z3_OP_ADD, Z3_OP_SUB, Z3_OP_MUL, Z3_OP_DIV, Z3_OP_IDIV, Z3_OP_MOD, Z3_OP_POWER,
    Z3_OP_LE, Z3_OP_LT, Z3_OP_GE, Z3_OP_GT, Z3_OP_BADD, Z3_OP_BSUB, Z3_OP_BMUL, Z3_OP_BSDIV, Z3_OP_BSMOD, Z3_OP_BOR, Z3_OP_BAND,
    Z3_OP_BXOR, Z3_OP_BSDIV, Z3_OP_SLEQ, Z3_OP_SLT, Z3_OP_SGEQ, Z3_OP_SGT, Z3_OP_BASHR, Z3_OP_BSHL
    ]

_z3_unary = [ Z3_OP_UMINUS, Z3_OP_BNOT, Z3_OP_BNEG ]

# Precedence
_z3_precedence = {
    Z3_OP_POWER : 0,
    Z3_OP_UMINUS : 1, Z3_OP_BNEG : 1, Z3_OP_BNOT : 1,
    Z3_OP_MUL : 2, Z3_OP_DIV : 2, Z3_OP_IDIV : 2, Z3_OP_MOD : 2, Z3_OP_BMUL : 2, Z3_OP_BSDIV : 2, Z3_OP_BSMOD : 2,
    Z3_OP_ADD : 3, Z3_OP_SUB : 3, Z3_OP_BADD : 3, Z3_OP_BSUB : 3,
    Z3_OP_BASHR : 4, Z3_OP_BSHL : 4,
    Z3_OP_BAND : 5,
    Z3_OP_BXOR : 6,
    Z3_OP_BOR : 7,
    Z3_OP_LE : 8, Z3_OP_LT : 8, Z3_OP_GE : 8, Z3_OP_GT : 8, Z3_OP_EQ : 8, Z3_OP_SLEQ : 8, Z3_OP_SLT : 8, Z3_OP_SGEQ : 8, Z3_OP_SGT : 8,
    Z3_OP_IFF : 8
    }

def _is_sum(k):
    return k == Z3_OP_ADD or k == Z3_OP_SUB or k == Z3_OP_BADD or k == Z3_OP_BSUB

_z3_infix_compact = [ Z3_OP_MUL, Z3_OP_BMUL, Z3_OP_POWER, Z3_OP_DIV, Z3_OP_IDIV, Z3_OP_MOD, Z3_OP_BSDIV, Z3_OP_BSMOD ]

_ellipses = '...'

_html_ellipses = '&hellip;'
# Overwrite some of the operators for HTML
_z3_pre_html_op_to_str = { Z3_OP_EQ : '=', Z3_OP_IFF : '=', Z3_OP_NOT : '&not;',
                           Z3_OP_AND : '&and;', Z3_OP_OR : '&or;', Z3_OP_IMPLIES : '&rArr;',
                           Z3_OP_LT : '&lt;', Z3_OP_GT : '&gt;', Z3_OP_LE : '&le;', Z3_OP_GE : '&ge;', 
                           Z3_OP_MUL : '&middot;', 
                           Z3_OP_SLEQ : '&le;', Z3_OP_SLT : '&lt;', Z3_OP_SGEQ : '&ge;', Z3_OP_SGT : '&gt;',
                           Z3_OP_ULEQ : '&le;<sub>u</sub>', Z3_OP_ULT : '&lt;<sub>u</sub>', 
                           Z3_OP_UGEQ : '&ge;<sub>u</sub>', Z3_OP_UGT : '&gt;<sub>u</sub>',
                           Z3_OP_BMUL : '&middot;',
                           Z3_OP_BUDIV : '/<sub>u</sub>', Z3_OP_BUREM : '%<sub>u</sub>', 
                           Z3_OP_BASHR : '&gt;&gt;', Z3_OP_BSHL : '&lt;&lt;',
                           Z3_OP_BLSHR : '&gt;&gt;<sub>u</sub>'
                           }

# Extra operators that are infix/unary for HTML
_z3_html_infix = [ Z3_OP_AND, Z3_OP_OR, Z3_OP_IMPLIES,
                   Z3_OP_ULEQ, Z3_OP_ULT, Z3_OP_UGEQ, Z3_OP_UGT, Z3_OP_BUDIV, Z3_OP_BUREM, Z3_OP_BLSHR
                   ]
_z3_html_unary = [ Z3_OP_NOT ]

# Extra Precedence for HTML
_z3_pre_html_precedence = { Z3_OP_BUDIV : 2, Z3_OP_BUREM : 2,
                            Z3_OP_BLSHR : 4,
                            Z3_OP_ULEQ : 8, Z3_OP_ULT : 8, 
                            Z3_OP_UGEQ : 8, Z3_OP_UGT : 8,
                            Z3_OP_ULEQ : 8, Z3_OP_ULT : 8, 
                            Z3_OP_UGEQ : 8, Z3_OP_UGT : 8,
                            Z3_OP_NOT : 1,
                            Z3_OP_AND : 10,
                            Z3_OP_OR  : 11,
                            Z3_OP_IMPLIES : 12 }

##############################
#
# End of Configuration
#
##############################

def _support_pp(a):
    return isinstance(a, z3.Z3PPObject) or isinstance(a, list) or isinstance(a, tuple)

_infix_map = {}
_unary_map = {}
_infix_compact_map = {}

for _k in _z3_infix:
    _infix_map[_k] = True
for _k in _z3_unary:
    _unary_map[_k] = True
for _k in _z3_infix_compact:
    _infix_compact_map[_k] = True

def _is_infix(k):
    global _infix_map
    return _infix_map.get(k, False)

def _is_infix_compact(k):
    global _infix_compact_map
    return _infix_compact_map.get(k, False)

def _is_unary(k):
    global _unary_map
    return _unary_map.get(k, False)

def _op_name(a):
    if isinstance(a, z3.FuncDeclRef):
        f = a
    else:
        f = a.decl()
    k = f.kind()
    n = _z3_op_to_str.get(k, None)
    if n == None:
        return f.name()
    else:
        return n

def _get_precedence(k):
    global _z3_precedence
    return _z3_precedence.get(k, 100000)

_z3_html_op_to_str = {}
for _k, _v in _z3_op_to_str.iteritems():
    _z3_html_op_to_str[_k] = _v
for _k, _v in _z3_pre_html_op_to_str.iteritems():
    _z3_html_op_to_str[_k] = _v

_z3_html_precedence = {}
for _k, _v in _z3_precedence.iteritems():
    _z3_html_precedence[_k] = _v
for _k, _v in _z3_pre_html_precedence.iteritems():
    _z3_html_precedence[_k] = _v

_html_infix_map = {}
_html_unary_map = {}
for _k in _z3_infix:
    _html_infix_map[_k] = True
for _k in _z3_html_infix:
    _html_infix_map[_k] = True
for _k in _z3_unary:
    _html_unary_map[_k] = True
for _k in _z3_html_unary:
    _html_unary_map[_k] = True

def _is_html_infix(k):
    global _html_infix_map
    return _html_infix_map.get(k, False)

def _is_html_unary(k):
    global _html_unary_map
    return _html_unary_map.get(k, False)

def _html_op_name(a):
    global _z3_html_op_to_str
    if isinstance(a, z3.FuncDeclRef):
        f = a
    else:
        f = a.decl()
    k = f.kind()
    n = _z3_html_op_to_str.get(k, None)
    if n == None:
        sym = Z3_get_decl_name(f.ctx_ref(), f.ast)
        if Z3_get_symbol_kind(f.ctx_ref(), sym) == Z3_INT_SYMBOL:
            return "&#950;<sub>%s</sub>" % Z3_get_symbol_int(f.ctx_ref(), sym)
        else:
            # Sanitize the string
            return f.name()
    else:
        return n

def _get_html_precedence(k):
    global _z3_html_predence
    return _z3_html_precedence.get(k, 100000)

class FormatObject:
    def is_compose(self):
        return False
    def is_choice(self):
        return False
    def is_indent(self):
        return False
    def is_string(self):
        return False
    def is_linebreak(self):
        return False
    def is_nil(self):
        return True
    def children(self):
        return []
    def as_tuple(self):
        return None
    def space_upto_nl(self):
        return (0, False)
    def flat(self):
        return self

class NAryFormatObject(FormatObject):
    def __init__(self, fs):
        assert all(map(lambda a: isinstance(a, FormatObject), fs))
        self.children = fs
    def children(self):
        return self.children

class ComposeFormatObject(NAryFormatObject):
    def is_compose(sef):
        return True
    def as_tuple(self):
        return ('compose', map(lambda a: a.as_tuple(), self.children))
    def space_upto_nl(self):
        r = 0
        for child in self.children:
            s, nl = child.space_upto_nl()
            r = r + s
            if nl:
                return (r, True)
        return (r, False)
    def flat(self):
        return compose(map(lambda a: a.flat(), self.children))

class ChoiceFormatObject(NAryFormatObject):
    def is_choice(sef):
        return True
    def as_tuple(self):
        return ('choice', map(lambda a: a.as_tuple(), self.children))
    def space_upto_nl(self):
        return self.children[0].space_upto_nl()
    def flat(self):
        return self.children[0].flat()

class IndentFormatObject(FormatObject):
    def __init__(self, indent, child):
        assert isinstance(child, FormatObject)
        self.indent = indent
        self.child  = child
    def children(self):
        return [self.child]
    def as_tuple(self):
        return ('indent', self.indent, self.child.as_tuple())
    def space_upto_nl(self):
        return self.child.space_upto_nl()
    def flat(self):
        return indent(self.indent, self.child.flat())
    def is_indent(self):
        return True

class LineBreakFormatObject(FormatObject):
    def __init__(self):
        self.space = ' '
    def is_linebreak(self):
        return True
    def as_tuple(self):
        return '<line-break>'
    def space_upto_nl(self):
        return (0, True)
    def flat(self):
        return to_format(self.space)

class StringFormatObject(FormatObject):
    def __init__(self, string):
        assert isinstance(string, str)
        self.string = string
    def is_string(self):
        return True
    def as_tuple(self):
        return self.string
    def space_upto_nl(self):
        return (getattr(self, 'size', len(self.string)), False)

def fits(f, space_left):
    s, nl = f.space_upto_nl()
    return s <= space_left

def to_format(arg, size=None):
    if isinstance(arg, FormatObject):
        return arg
    else:
        r = StringFormatObject(str(arg))
        if size != None:
            r.size = size
        return r

def compose(*args):
    if len(args) == 1 and (isinstance(args[0], list) or isinstance(args[0], tuple)):
        args = args[0]
    return ComposeFormatObject(args)

def indent(i, arg):
    return IndentFormatObject(i, arg)

def group(arg):
    return ChoiceFormatObject([arg.flat(), arg])

def line_break():
    return LineBreakFormatObject()

def _len(a):
    if isinstance(a, StringFormatObject):
        return getattr(a, 'size', len(a.string))
    else:
        return len(a)

def seq(args, sep=',', space=True):
    nl = line_break()
    if not space:
        nl.space = ''
    r = []
    r.append(args[0])
    num  = len(args)
    for i in range(num - 1):
        r.append(to_format(sep))
        r.append(nl)
        r.append(args[i+1])
    return compose(r)

def seq1(header, args, lp='(', rp=')'):
    return group(compose(to_format(header),
                         to_format(lp),
                         indent(len(lp) + _len(header),
                                seq(args)),
                         to_format(rp)))

def seq2(header, args, i=4, lp='(', rp=')'):
    if len(args) == 0:
        return compose(to_format(header), to_format(lp), to_format(rp))
    else:
        return group(compose(indent(len(lp), compose(to_format(lp), to_format(header))),
                             indent(i, compose(seq(args), to_format(rp)))))

def seq3(args, lp='(', rp=')'):
    if len(args) == 0:
        return compose(to_format(lp), to_format(rp))
    else:
        return group(indent(len(lp), compose(to_format(lp), seq(args), to_format(rp))))

class StopPPException:
    def __str__(self):
        return 'pp-interrupted'

class PP:
    def __init__(self):
        self.max_lines  = 200
        self.max_width  = 60
        self.bounded    = False
        self.max_indent = 40
        
    def pp_string(self, f, indent):
        if not self.bounded or self.pos <= self.max_width:
            sz = _len(f)
            if self.bounded and self.pos + sz > self.max_width:
                self.out.write(_ellipses)
            else:
                self.pos = self.pos + sz
                self.ribbon_pos = self.ribbon_pos + sz
                self.out.write(unicode(f.string))

    def pp_compose(self, f, indent):
        for c in f.children:
            self.pp(c, indent)

    def pp_choice(self, f, indent):
        space_left = self.max_width - self.pos
        if space_left > 0 and fits(f.children[0], space_left):
            self.pp(f.children[0], indent)
        else:
            self.pp(f.children[1], indent)
    
    def pp_line_break(self, f, indent):
        self.pos = indent
        self.ribbon_pos = 0
        self.line = self.line + 1
        if self.line < self.max_lines: 
            self.out.write(unicode('\n'))
            for i in range(indent):
                self.out.write(unicode(' '))
        else:
            self.out.write(unicode('\n...'))
            raise StopPPException()

    def pp(self, f, indent):
        if f.is_string():
            self.pp_string(f, indent)
        elif f.is_indent():
            self.pp(f.child, min(indent + f.indent, self.max_indent))
        elif f.is_compose():
            self.pp_compose(f, indent)
        elif f.is_choice():
            self.pp_choice(f, indent)
        elif f.is_linebreak():
            self.pp_line_break(f, indent)
        else:
            return

    def __call__(self, out, f):
        try:
            self.pos = 0
            self.ribbon_pos = 0
            self.line = 0
            self.out  = out
            self.pp(f, 0)
        except StopPPException:
            return
    
class Formatter:
    def __init__(self):
        global _ellipses
        self.max_depth           = 20
        self.max_args            = 128
        self.rational_to_decimal = False
        self.precision           = 10
        self.ellipses            = to_format(_ellipses)
        self.max_visited         = 10000
    
    def pp_ellipses(self):
        return self.ellipses

    def pp_arrow(self):
        return ' ->'

    def pp_unknown(self):
        return '<unknown>'
        
    def pp_name(self, a):
        return to_format(_op_name(a))

    def is_infix(self, a):
        return _is_infix(a)
    
    def is_unary(self, a):
        return _is_unary(a)

    def get_precedence(self, a):
        return _get_precedence(a)

    def is_infix_compact(self, a):
        return _is_infix_compact(a)

    def is_infix_unary(self, a):
        return self.is_infix(a) or self.is_unary(a) 

    def add_paren(self, a):
        return compose(to_format('('), indent(1, a), to_format(')'))
    
    def pp_sort(self, s):
        if isinstance(s, z3.ArraySortRef):
            return seq1('Array', (self.pp_sort(s.domain()), self.pp_sort(s.range())))
        elif isinstance(s, z3.BitVecSortRef):
            return seq1('BitVec', (to_format(s.size()), ))
        else:
            return to_format(s.name())

    def pp_const(self, a):
        return self.pp_name(a)

    def pp_int(self, a):
        return to_format(a.as_string())
        
    def pp_rational(self, a):
        if not self.rational_to_decimal:
            return to_format(a.as_string())
        else:
            return to_format(a.as_decimal(self.precision))

    def pp_algebraic(self, a):
        return to_format(a.as_decimal(self.precision))

    def pp_bv(self, a):
        return to_format(a.as_string())

    def pp_prefix(self, a, d, xs):
        r  = []
        sz = 0
        for child in a.children(): 
            r.append(self.pp_expr(child, d+1, xs))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        return seq1(self.pp_name(a), r)

    def infix_args_core(self, a, d, xs, r):
        sz = len(r)
        k  = a.decl().kind()
        p  = self.get_precedence(k)
        for child in a.children():
            child_pp = self.pp_expr(child, d+1, xs)
            child_k = None
            if z3.is_app(child):
                child_k = child.decl().kind()
            if k == child_k:
                self.infix_args_core(child, d, xs, r)
                sz = len(r)
                if sz > self.max_args:
                    return
            elif self.is_infix_unary(child_k):
                child_p = self.get_precedence(child_k)
                if p > child_p or (_is_sum(k) and _is_sum(child_k)):
                    r.append(child_pp)
                else: 
                    r.append(self.add_paren(child_pp))
                sz = sz + 1
            elif z3.is_quantifier(child):
                r.append(self.add_paren(child_pp))
            else:
                r.append(child_pp)
                sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                return

    def infix_args(self, a, d, xs):
        r = []
        self.infix_args_core(a, d, xs, r)
        return r

    def pp_infix(self, a, d, xs):
        k  = a.decl().kind()
        if self.is_infix_compact(k):
            op = self.pp_name(a)
            return group(seq(self.infix_args(a, d, xs), op, False))
        else:
            op = self.pp_name(a)
            sz = _len(op)
            op.string = ' ' + op.string
            op.size   = sz + 1
            return group(seq(self.infix_args(a, d, xs), op))

    def pp_unary(self, a, d, xs):
        k  = a.decl().kind()
        p  = self.get_precedence(k)
        child    = a.children()[0]
        child_k  = None
        if z3.is_app(child):
            child_k = child.decl().kind()
        child_pp = self.pp_expr(child, d+1, xs)        
        if k != child_k and self.is_infix_unary(child_k):
            child_p = self.get_precedence(child_k)
            if p <= child_p:
                child_pp = self.add_paren(child_pp)
        if z3.is_quantifier(child):
            child_pp = self.add_paren(child_pp)
        name = self.pp_name(a)
        return compose(to_format(name), indent(_len(name), child_pp))

    def pp_power_arg(self, arg, d, xs):
        r = self.pp_expr(arg, d+1, xs)
        k = None
        if z3.is_app(arg):
            k = arg.decl().kind()
        if self.is_infix_unary(k) or (z3.is_rational_value(arg) and arg.denominator_as_long() != 1):
            return self.add_paren(r)
        else:
            return r

    def pp_power(self, a, d, xs):
        arg1_pp = self.pp_power_arg(a.arg(0), d+1, xs)
        arg2_pp = self.pp_power_arg(a.arg(1), d+1, xs)
        return group(seq((arg1_pp, arg2_pp), '**', False))

    def pp_neq(self):
        return to_format("!=")

    def pp_distinct(self, a, d, xs):
        if a.num_args() == 2:
            op = self.pp_neq()
            sz = _len(op)
            op.string = ' ' + op.string
            op.size   = sz + 1
            return group(seq(self.infix_args(a, d, xs), op))
        else:
            return self.pp_prefix(a, d, xs)

    def pp_select(self, a, d, xs):
        if a.num_args() != 2:
            return self.pp_prefix(a, d, xs)
        else:
            arg1_pp = self.pp_expr(a.arg(0), d+1, xs)
            arg2_pp = self.pp_expr(a.arg(1), d+1, xs)
            return compose(arg1_pp, indent(2, compose(to_format('['), arg2_pp, to_format(']'))))

    def pp_unary_param(self, a, d, xs):
        p   = Z3_get_decl_int_parameter(a.ctx_ref(), a.decl().ast, 0)
        arg = self.pp_expr(a.arg(0), d+1, xs)
        return seq1(self.pp_name(a), [ to_format(p), arg ])

    def pp_extract(self, a, d, xs):
        h   = Z3_get_decl_int_parameter(a.ctx_ref(), a.decl().ast, 0)
        l   = Z3_get_decl_int_parameter(a.ctx_ref(), a.decl().ast, 1)
        arg = self.pp_expr(a.arg(0), d+1, xs)
        return seq1(self.pp_name(a), [ to_format(h), to_format(l), arg ])

    def pp_pattern(self, a, d, xs):
        if a.num_args() == 1:
            return self.pp_expr(a.arg(0), d, xs)
        else:
            return seq1('MultiPattern', [ self.pp_expr(arg, d+1, xs) for arg in a.children() ])

    def pp_map(self, a, d, xs):
        r  = []
        sz = 0
        f  = z3.get_map_func(a)
        r.append(to_format(f.name()))
        for child in a.children(): 
            r.append(self.pp_expr(child, d+1, xs))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        return seq1(self.pp_name(a), r)

    def pp_K(self, a, d, xs):
        return seq1(self.pp_name(a), [ self.pp_sort(a.domain()), self.pp_expr(a.arg(0), d+1, xs) ])

    def pp_app(self, a, d, xs):
        if z3.is_int_value(a):
            return self.pp_int(a)
        elif z3.is_rational_value(a):
            return self.pp_rational(a)
        elif z3.is_algebraic_value(a):
            return self.pp_algebraic(a)
        elif z3.is_bv_value(a):
            return self.pp_bv(a)
        elif z3.is_const(a):
            return self.pp_const(a)
        else:
            f = a.decl()
            k = f.kind()
            if k == Z3_OP_POWER:
                return self.pp_power(a, d, xs)
            elif k == Z3_OP_DISTINCT:
                return self.pp_distinct(a, d, xs)
            elif k == Z3_OP_SELECT:
                return self.pp_select(a, d, xs)
            elif k == Z3_OP_SIGN_EXT or k == Z3_OP_ZERO_EXT or k == Z3_OP_REPEAT:
                return self.pp_unary_param(a, d, xs)
            elif k == Z3_OP_EXTRACT:
                return self.pp_extract(a, d, xs)
            elif k == Z3_OP_ARRAY_MAP:
                return self.pp_map(a, d, xs)
            elif k == Z3_OP_CONST_ARRAY:
                return self.pp_K(a, d, xs)
            elif z3.is_pattern(a):
                return self.pp_pattern(a, d, xs)
            elif self.is_infix(k):
                return self.pp_infix(a, d, xs)
            elif self.is_unary(k):
                return self.pp_unary(a, d, xs)
            else:
                return self.pp_prefix(a, d, xs)

    def pp_var(self, a, d, xs):
        idx = z3.get_var_index(a)
        sz  = len(xs)
        if idx >= sz:
            return seq1('Var', (to_format(idx),))
        else:
            return to_format(xs[sz - idx - 1])
        
    def pp_quantifier(self, a, d, xs):
        ys = [ to_format(a.var_name(i)) for i in range(a.num_vars()) ]
        new_xs  = xs + ys
        body_pp = self.pp_expr(a.body(), d+1, new_xs)
        if len(ys) == 1:
            ys_pp = ys[0]
        else:
            ys_pp   = seq3(ys, '[', ']')
        if a.is_forall():
            header = 'ForAll'
        else:
            header = 'Exists'
        return seq1(header, (ys_pp, body_pp))

    def pp_expr(self, a, d, xs):
        self.visited = self.visited + 1
        if d > self.max_depth or self.visited > self.max_visited:
            return self.pp_ellipses()
        if z3.is_app(a):
            return self.pp_app(a, d, xs)
        elif z3.is_quantifier(a):
            return self.pp_quantifier(a, d, xs)
        elif z3.is_var(a):
            return self.pp_var(a, d, xs)
        else:
            return to_format(self.pp_unknown())

    def pp_seq_core(self, f, a, d, xs):
        self.visited = self.visited + 1
        if d > self.max_depth or self.visited > self.max_visited:
            return self.pp_ellipses()
        r = []
        sz = 0
        for elem in a:
            r.append(f(elem, d+1, xs))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        return seq3(r, '[', ']')
        
    def pp_seq(self, a, d, xs):
        return self.pp_seq_core(self.pp_expr, a, d, xs)

    def pp_seq_seq(self, a, d, xs):
        return self.pp_seq_core(self.pp_seq, a, d, xs)

    def pp_model(self, m):
        r = []
        sz = 0
        for d in m:
            i = m[d]
            if isinstance(i, z3.FuncInterp):
                i_pp = self.pp_func_interp(i)
            else:
                i_pp = self.pp_expr(i, 0, [])
            name = self.pp_name(d)
            r.append(compose(name, to_format(' = '), indent(_len(name) + 3, i_pp)))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        return seq3(r, '[', ']')

    def pp_func_entry(self, e):
        num = e.num_args()
        if num > 1:
            args = []
            for i in range(num):
                args.append(self.pp_expr(e.arg_value(i), 0, []))
            args_pp = group(seq3(args))
        else:
            args_pp   = self.pp_expr(e.arg_value(0), 0, [])
        value_pp = self.pp_expr(e.value(), 0, []) 
        return group(seq((args_pp, value_pp), self.pp_arrow()))

    def pp_func_interp(self, f):
        r = []
        sz = 0
        num = f.num_entries()
        for i in range(num):
            r.append(self.pp_func_entry(f.entry(i)))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        if sz <= self.max_args:
            else_pp = self.pp_expr(f.else_value(), 0, [])
            r.append(group(seq((to_format('else'), else_pp), self.pp_arrow())))
        return seq3(r, '[', ']')

    def pp_list(self, a):
        r = []
        sz = 0
        for elem in a:
            if _support_pp(elem):
                r.append(self.main(elem))
            else:
                r.append(to_format(str(elem)))
            sz = sz + 1
            if sz > self.max_args:
                r.append(self.pp_ellipses())
                break
        if isinstance(a, tuple):
            return seq3(r)
        else:
            return seq3(r, '[', ']')

    def main(self, a):
        if z3.is_expr(a):
            return self.pp_expr(a, 0, [])
        elif z3.is_sort(a):
            return self.pp_sort(a)
        elif z3.is_func_decl(a):
            return self.pp_name(a)
        elif isinstance(a, z3.Goal) or isinstance(a, z3.AstVector):
            return self.pp_seq(a, 0, [])
        elif isinstance(a, z3.Solver):
            return self.pp_seq(a.assertions(), 0, [])
        elif isinstance(a, z3.Fixedpoint):
            return a.sexpr()
        elif isinstance(a, z3.ApplyResult):
            return self.pp_seq_seq(a, 0, [])
        elif isinstance(a, z3.ModelRef):
            return self.pp_model(a)
        elif isinstance(a, z3.FuncInterp):
            return self.pp_func_interp(a)
        elif isinstance(a, list) or isinstance(a, tuple):
            return self.pp_list(a)
        else:
            return to_format(self.pp_unknown())

    def __call__(self, a):
        self.visited = 0
        return self.main(a)

class HTMLFormatter(Formatter):
    def __init__(self):
        Formatter.__init__(self)
        global _html_ellipses
        self.ellipses = to_format(_html_ellipses)

    def pp_arrow(self):
        return to_format(' &rarr;', 1)

    def pp_unknown(self):
        return '<b>unknown</b>'
        
    def pp_name(self, a):
        r = _html_op_name(a)
        if r[0] == '&' or r[0] == '/' or r[0] == '%':
            return to_format(r, 1)
        else:
            pos = r.find('__')
            if pos == -1 or pos == 0:
                return to_format(r)
            else:
                sz = len(r)
                if pos + 2 == sz:
                    return to_format(r)
                else:
                    return to_format('%s<sub>%s</sub>' % (r[0:pos], r[pos+2:sz]), sz - 2)

    def is_infix(self, a):
        return _is_html_infix(a)
    
    def is_unary(self, a):
        return _is_html_unary(a)

    def get_precedence(self, a):
        return _get_html_precedence(a)

    def pp_neq(self):
        return to_format("&ne;")

    def pp_power(self, a, d, xs):
        arg1_pp = self.pp_power_arg(a.arg(0), d+1, xs)
        arg2_pp = self.pp_expr(a.arg(1), d+1, xs)
        return compose(arg1_pp, to_format('<sup>', 1), arg2_pp, to_format('</sup>', 1)) 

    def pp_var(self, a, d, xs):
        idx = z3.get_var_index(a)
        sz  = len(xs)
        if idx >= sz:
            # 957 is the greek letter nu
            return to_format('&#957;<sub>%s</sub>' % idx, 1)
        else:
            return to_format(xs[sz - idx - 1])

    def pp_quantifier(self, a, d, xs):
        ys = [ to_format(a.var_name(i)) for i in range(a.num_vars()) ]
        new_xs  = xs + ys
        body_pp = self.pp_expr(a.body(), d+1, new_xs)
        ys_pp = group(seq(ys))
        if a.is_forall():
            header = '&forall;'
        else:
            header = '&exist;'
        return group(compose(to_format(header, 1), 
                             indent(1, compose(ys_pp, to_format(' :'), line_break(), body_pp))))


_PP        = PP()
_Formatter = Formatter()

def set_pp_option(k, v):
    if k == 'html_mode':
        if v:
            set_html_mode(True)
        else:
            set_html_mode(False)
        return True
    val = getattr(_PP, k, None)
    if val != None:
        z3._z3_assert(type(v) == type(val), "Invalid pretty print option value")
        setattr(_PP, k, v)
        return True
    val = getattr(_Formatter, k, None)
    if val != None:
        z3._z3_assert(type(v) == type(val), "Invalid pretty print option value")
        setattr(_Formatter, k, v)
        return True
    return False

def obj_to_string(a):
    out = io.StringIO()
    _PP(out, _Formatter(a))
    return out.getvalue()

_html_out = None

def set_html_mode(flag=True):
    global _Formatter
    if flag:
        _Formatter = HTMLFormatter()
    else:
        _Formatter = Formatter()

def in_html_mode():
    return isinstance(_Formatter, HTMLFormatter)

def pp(a):
    if _support_pp(a):
        print obj_to_string(a)
    else:
        print a

def print_matrix(m):
    z3._z3_assert(isinstance(m, list) or isinstance(m, tuple), "matrix expected")
    if not in_html_mode():
        print obj_to_string(m)
    else:
        print '<table cellpadding="2", cellspacing="0", border="1">'
        for r in m:
            z3._z3_assert(isinstance(r, list) or isinstance(r, tuple), "matrix expected")
            print '<tr>'
            for c in r:
                print '<td>%s</td>' % c
            print '</tr>'
        print '</table>'
    
def insert_line_breaks(s, width):
    """Break s in lines of size width (approx)"""
    sz = len(s)
    if sz <= width:
        return s
    new_str = io.StringIO()
    w = 0
    for i in range(sz):
        if w > width and s[i] == ' ':
            new_str.write(u'<br />')
            w = 0
        else:
            new_str.write(unicode(s[i]))
        w = w + 1
    return new_str.getvalue()
