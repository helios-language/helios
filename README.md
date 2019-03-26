
[![CircleCI](https://circleci.com/gh/helios-language/helios.svg?style=svg)](https://circleci.com/gh/helios-language/helios)

# Helios

Documentation: https://helios-language.github.io/helios/ <br>
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
a = function(a,b)->int: 
    return a + b


```

namespaces:

```

#create namespace
somename:
    a = 3
    b = 4

print(somename.a)

#reverse of a namespace, makes code from a namespace with a long name accessible with a short name. 
using something.something.something as s:
    s.something()

```

# if statements

```

if a:
    dosomething()

    if b:
        stop # No else will be executed. This if statement has ended. Like a return in an if.
    if c:
        dosomethingelse()
    if d:
        doanotherthing()
    
elif:
    yeet()
else:
    yeet(42)

#if as an assignment

a = if b:
    something
elif c:
    somethingelse
elif d:
    dosomething()
    somethingelse = something()
    stop somethingelse
elif e:
    dosomethingelse()
    something # last statement is implicit stop
else: # if assignment needs else!
    42
    
```

# threads

```

thread:
    sleep(10)
    print("yeet")

print(42) # this is executed first


# explicit access to threads:

a = thread:
    sleep(10)
    print("yeet")

a.join() # "yeet" is printed first but after 10 seconds.
print(42)


a = 10
thread:
    print(a) # error. this variable isnt owned by this thread
    
a = 5
print(a)


a = 10
thread a:
    print(a)
    
a = 5  # error. this variable isnt owned by this thread
print(a)

# There is no syntax for tunnels/channels/streams yet but this will come for inter thread come. Suggestions are welcome.


```


# memory usage and garbage collection

![](resources/memory%20useage%20of%20a%20simple%20expression.png)

This is an example of a normal memory usage. It seems pretty high but the vm assumes large expressions and automatically allocates a kilobyte of memory for the internal execution stack, as allocating more at once is more efficient. The memory usage for the hvm part will therefore be pretty constant. 

![](resources/memory%20usage%20two%20phases.png)

As is very apparent in this image, there are two phases in the running of a helios program. the compilation and the execution. Both take up some memory but when the execution start most of the memory used by the compiler is freed.



note: The helios language spec will only allow either tabs or 42 spaces/tab. The default 4 spaces per tab    will not be allowed as tabs are clearly superior.
