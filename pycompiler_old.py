#!/usr/bin/env python3
import time
import ast


class Error:
    def __init__(self, message, line, char):
        self.message = message
        self.line = line
        self.char = char

    def throw(self, code=None, hard=True):
        if code == None:
            print(self.__repr__())
            if hard:
                exit()
            else:
                return

        else:
            # show line etc
            print(
                """
{}
{}
{}
            """ .format(
                    self.__repr__(),
                    code.getIteree().split("\n")[self.line],
                    " "*self.char + "^"
                )
            )
            if hard:
                exit()
            else:
                return

    def __repr__(self):
        return self.message + " in line {} character {}".format(self.line, self.char)


class ErrorStack:
    stack = []

    def __init__(self):
        raise NotImplementedError

    @classmethod
    def error(cls, message):
        # print("error stack extended with {}".format(message))
        cls.stack.append(message)

    @classmethod
    def getCurrentLength(cls):
        return len(cls.stack)

    @classmethod
    def popUntilLength(cls, length):
        while cls.getCurrentLength() > length:
            rem = cls.stack.pop()
            # print("removed {}".format(rem))

    @classmethod
    def top(cls):
        return cls.stack[-1]

    @classmethod
    def empty(cls):
        return cls.getCurrentLength() == 0

    @classmethod
    def traceback(cls, code=None):
        for i in cls.stack[-1:0:-1]:
            i.throw(hard=False, code=None)
        cls.top().throw(code=code, hard=True)


class Token:
    def __init__(self, value):
        self.value = value

    def __repr__(self):
        return "Token ({}):{}".format(self.__class__.__name__, self.value)

    def __eq__(self, other):
        return self.__class__ == other.__class__ and self.value == other.value

    # def __hash__(self):
    #     return hash(self.value) ^ hash(self.__class__.__name__) << 3


class Operator(Token):
    pass


class BinaryOperator(Operator):
    pass


class UnaryOperator(Operator):
    pass


class Integer(Token):
    pass


class HexInteger(Integer):
    pass


class BinInteger(Integer):
    pass


class OctInteger(Integer):
    pass


class Float(Token):
    pass


class AST:
    def __init__(self, value, *children):
        self.children = children
        self.value = value

        assert not any([i == None for i in self.children]
                       ), "parser error, ast got None"

    def __repr__(self, indent=0):
        childrenstring = "\n" + "\n".join(
            (i.__repr__(indent + 1) for i in self.children if i != None)
        )

        return "{}{}{}".format(
            "\t" * indent,
            self.value,
            childrenstring if childrenstring != "\n" else ""
        )

    def addchild(self, child):
        self.children.append(child)

    def addchildren(self, *children):
        for i in children:
            self.addchild(i)


class CopyableIterator:
    def __init__(self, it, index=0, prev=None, curr=None, next=None, char=-1, line=0):
        self.it = it
        self.index = index

        self.curr = curr
        self.next = next
        self.prev = prev

        self.char = char
        self.line = line

    def getIteree(self):
        return self.it

    def __iter__(self):
        return self

    def __next__(self):
        self.prev = self.curr
        self.curr = self.next
        try:
            self.next = self.it[self.index]
        except IndexError:
            self.next = None

        self.char += 1
        if self.next == "\n":
            self.line += 1
            self.char = 0

        self.index += 1

        return self

    def exhausted(self):
        return self.next == None

    def copy(self):
        return self.__class__(self.it, self.index, self.prev, self.curr, self.next, self.char, self.line)

    def __repr__(self):
        return "Iterator: {} {} {}".format(self.prev, self.curr, self.next)


class ParserManager:
    def __init__(self, code=None):
        if isinstance(code, str):
            self.parse(code)

    def parse(self, code):
        self.code = CopyableIterator(code)

        self.curr = None
        self.next = None
        self.prev = None
        self.advance()

        res = self.root()
        if not self.code.exhausted():
            ErrorStack.error(Error(
                "Invalid syntax",
                self.code.line,
                self.code.char
            ))

        if not ErrorStack.empty():
            ErrorStack.traceback(self.code)
        return res

    def skipws(self):
        while self.code.next and self.code.next in "\t ":
            self.advance()

    def skipwsnl(self):
        while self.code.next and self.code.next in "\n\t ":
            self.advance()

    def advance(self):
        next(self.code)

    def acceptchar(self, character):
        if self.code.next != None and self.code.next == character:
            self.advance()
            self.code.accepted = character
            return True
        return False

    def acceptanychar(self, characters):

        for i in characters:
            if self.acceptchar(i):
                self.code.accepted = i
                return True

        return False

    def expectchar(self, character):
        if not self.acceptchar(character):
            ErrorStack.error(Error(
                "expected {}".format(character),
                self.code.line,
                self.code.char
            ))
            return False
        return True

    def acceptstring(self, string):
        codecp1 = self.code.copy()
        for i in string:
            if not self.acceptchar(i):
                self.code = codecp1.copy()  # restore if False
                return False
        self.code.accepted = string
        return True

    def expectstring(self, string):
        codecp1 = self.code.copy()

        for i in string:
            if not self.expectchar(i):
                ErrorStack.error(Error(
                    "expected {}".format(string),
                    codecp1.line,
                    codecp1.char
                ))
                return False
        return True

    def __repr__(self):
        return "parser with codeiterator {}".format(str(self.code))


class Parser(ParserManager):
    def root(self):
        return self.expr()

    def expr(self):
        self.skipws()
        res = self.term()
        self.skipws()
        while self.acceptstring("+") or self.acceptstring("-"):
            self.skipws()
            op = self.code.accepted
            self.skipws()
            right = self.term()
            self.skipws()
            if op in {"+", "-"}:
                res = AST(BinaryOperator(op), res, right)
            else:
                ErrorStack.error(Error(
                    "invalid operator {}".format(op),
                    self.code.line,
                    self.code.char
                ))
                break

        return res

    def term(self):
        self.skipws()
        res = self.factor()
        self.skipws()

        while self.acceptstring("*") or self.acceptstring("//") or self.acceptstring("/"):
            self.skipws()

            op = self.code.accepted

            self.skipws()

            right = self.factor()

            self.skipws()

            if op in {"*", "/", "//"}:
                res = AST(BinaryOperator(op), res, right)
            else:
                ErrorStack.error(Error(
                    "invalid operator {}".format(op),
                    self.code.line,
                    self.code.char,
                ))
                break

        return res

    def factor(self):
        self.skipws()
        if self.acceptanychar("+-~"):
            self.skipws()
            op = self.code.accepted
            self.skipws()
            child = self.factor()
            self.skipws()
            if op in {"+", "-", "~"}:
                return AST(UnaryOperator(op), child)
            else:
                ErrorStack.error(Error(
                    "invalid operator {}".format(op),
                    self.code.line,
                    self.code.char
                ))

        return self.power()

    def power(self):
        self.skipws()
        res = self.atomexpression()
        self.skipws()

        if self.acceptstring("**"):
            self.skipws()
            op = self.code.accepted
            self.skipws()
            right = self.power()
            self.skipws()
            if op in ["**"]:
                res = AST(BinaryOperator(op), res, right)
            else:
                ErrorStack.error(Error(
                    "invalid operator {}".format(op),
                    self.code.line,
                    self.code.char
                ))

        self.skipws()
        return res

    def atomexpression(self):
        return self.bracketexpression()

    def bracketexpression(self):
        self.skipws()

        if self.acceptchar("("):
            self.skipws()
            res = self.expr()
            self.skipws()
            self.expectchar(")")
            self.skipws()
            return res
        else:
            return self.atom()

    def atom(self):
        self.skipws()
        length = ErrorStack.getCurrentLength()
        codecp1 = self.code.copy()

        res = self.floatconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        res = self.intconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        # ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        ErrorStack.error(Error(
            "could not parse atom",
            self.code.line, self.code.char
        ))
        return AST("")

    def floatconst(self):
        self.skipws()

        length1 = ErrorStack.getCurrentLength()
        codecp1 = self.code.copy()

        ipart = self.intconst().value
        if ErrorStack.getCurrentLength() != length1:
            ipart = 0
            ErrorStack.popUntilLength(length1)
            self.code = codecp1.copy()

        if not self.expectchar("."):
            self.skipws()
            if self.acceptstring("exp") or self.acceptstring("e"):
                ErrorStack.popUntilLength(length1)
                self.skipws()
                length4 = ErrorStack.getCurrentLength()
                epart = self.intconst()
                if ErrorStack.getCurrentLength() != length4:
                    ErrorStack.error(Error(
                        "expected integer constant after exp",
                        self.code.line, self.code.char
                    ))
                    return
                return AST(Float((ipart, 0, epart)))

            ErrorStack.error(Error(
                "invalid float constant",
                self.code.line, self.code.char
            ))
            return

        length2 = ErrorStack.getCurrentLength()
        codecp2 = self.code.copy()

        fpart = self.intconst().value
        if ErrorStack.getCurrentLength() != length2:
            fpart = 0
            ErrorStack.popUntilLength(length2)
            self.code = codecp2.copy()

        self.skipws()
        if self.acceptstring("exp") or self.acceptstring("e"):
            length3 = ErrorStack.getCurrentLength()
            self.skipws()
            codecp3 = self.code.copy()
            epart = self.intconst()
            if ErrorStack.getCurrentLength() != length3:
                ErrorStack.error(Error(
                    "expected integer constant after exp",
                    self.code.line, self.code.char
                ))
                return
        else:
            epart = 0

        return AST(Float((ipart, fpart, epart)))

    def intconst(self):

        self.skipws()

        length = ErrorStack.getCurrentLength()
        codecp1 = self.code.copy()

        res = self.hexconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        res = self.binconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        res = self.octconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        res = self.decconst()
        if ErrorStack.getCurrentLength() == length:
            return res

        # ErrorStack.popUntilLength(length)
        self.code = codecp1.copy()

        ErrorStack.error(Error(
            "invalid integer constant",
            self.code.line, self.code.char
        ))
        return AST("")

    def decconst(self):
        # optional 0d
        self.acceptstring("0d")

        if not self.acceptanychar("0123456789"):
            ErrorStack.error(Error(
                "invalid dec const",
                self.code.line,
                self.code.char
            ))
            return AST("")
        res = [self.code.accepted]
        while self.acceptanychar("0123456789"):
            res.append(self.code.accepted)

        return AST(Integer("".join(res)))

    def octconst(self):
        if not self.expectstring("0o"):
            ErrorStack.error(Error(
                "invalid oct const",
                self.code.line,
                self.code.char
            ))
            return AST("")

        if not self.acceptanychar("01234567"):
            ErrorStack.error(Error(
                "invalid oct const",
                self.code.line,
                self.code.char
            ))
            return AST("")
        res = [self.code.accepted]
        while self.acceptanychar("01234567"):
            res.append(self.code.accepted)
        return AST(OctInteger("".join(res)))

    def binconst(self):
        if not self.expectstring("0b"):
            ErrorStack.error(Error(
                "invalid bin const",
                self.code.line,
                self.code.char
            ))
            return AST("")

        if not self.acceptanychar("01"):
            ErrorStack.error(Error(
                "invalid bin const",
                self.code.line,
                self.code.char
            ))
            return AST("")
        res = [self.code.accepted]
        while self.acceptanychar("01"):
            res.append(self.code.accepted)
        return AST(BinInteger("".join(res)))

    def hexconst(self):
        if not self.expectstring("0x"):
            ErrorStack.error(Error(
                "invalid hex const",
                self.code.line,
                self.code.char
            ))
            return AST("")

        if not self.acceptanychar("0123456789abcdefABCDEF"):
            ErrorStack.error(Error(
                "invalid hex const",
                self.code.line,
                self.code.char
            ))
            return AST("")
        res = [self.code.accepted]
        while self.acceptanychar("0123456789abcdefABCDEF"):
            res.append(self.code.accepted)
        return AST(HexInteger("".join(res)))


p = Parser()

print(p.parse("(1 + 2) / 3 + 0x4 * -5 ** 6 // 3"))
