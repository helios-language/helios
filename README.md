
[![CircleCI](https://circleci.com/gh/helios-language/helios.svg?style=svg)](https://circleci.com/gh/helios-language/helios)

# Helios

Documentation: https://helios-language.github.io/helios/
Github repository: https://github.com/helios-language/helios/

Helios is a (very) WIP programming language that aims to take the flexebility of scripting languages (inspired by python) and add to it a system of more static typing. This means it will still make typing optional, but when it's used it is enforced throughout the program.

# contribution

Any contribution is welcome - be it in the form of programming support, new language ideas, bug fixes or anything else. As a full time student this is definitely an (although active) side project. I leave many TODO: notices in my code so that's also a way to add something.

> At this moment only the parser is being developed. I have been working on ideas for the actual VM/JIT that will execute code but thats WIP.

# examples of implemented mechanics:

expression parsing:

```
3 + 0b01001 * 0o6 / 0x6f ** -3
```

well that's about it tbh...

# examples of WIP or future mechanics

typing system:
```
a->int = 5

b->int = 6
b = 0.1 #type error

c -> (int,float) = 3
c = 0.1 #fine
c = "hello" #type error

b = 0.1
a = 3 + b #fine (a is untyped)
a->int = 3 + b #type error (result is float)
a->int = 3 + b->int #fine

```

functions and blocks
```

#demonstrates parameter typing and return types and default parameters
function helloworld(a,b->int,c->int=8)->float:
    a + b / c #return not required for the last statement in the function (but allowed)

#lambdas
a = function(a,b)->int: c #no newline allowed (since brackets wont be allowed and i dont have a better solution)


```

extension of blocks:

```

using something.something.something as s:
    s.something()

#note: basically makes a proxy to the long name referenced by  the short name for the duration of the block.
#on enter the __enter__ method of the something.something.something will be called (if present)
#on exit the __exit__ method will be called in a  similar fashion.
#when an error occures in this block the __error__ function will be called and the block can act like a try-catch statement
#WIP

#namespaces:

somename:
    a = 3
    b = 4

print(somename.a)

```
