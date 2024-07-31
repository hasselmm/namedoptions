Named Options for C++
=====================

Motivation
----------

One of the last remaining shortcomings of C++ is that functions
arguments cannot be named. This makes it hard to read certain function
calls. Just one real-world example from Qt:

```C++
auto *const layout = new QGridLayout;
layout->addWidget(widget, 1, 2, 3, 4, Qt::AlignCenter);
```


What's the meaning of all these magic numbers? In Python and many other
modern programming languages one could do better[^Pyside]:

```Python
layout = QGridLayout()
layout.addWidget(widget, row=1, column=2, rowSpan=3, columnSpan=4, alignment=Qt::AlignCenter);
```

[^Pyside]: Just that Qt's Python bindings do not use this possibility.

The canonical workaround in C++ is to introduce an options object:

```C++
struct AddWidgetOptions
{
    int row;            // fields without initializer are mandatory
    int column;
    int rowSpan = 1;    // initialized fields are optional
    int columnSpan = 1;
    Qt::Alignment alignment = Qt::AlignCenter;
};
```

With traditional C++ this options object would be used like this:

```C++
auto options = QGridLayout::AddWidgetOptions{};
options.row = 1;
options.column = 2;

layout->addWidget(widget, options);
```


Not really nice, but C++ 20 and its designated initializers come to rescue:

```C++
layout->addWidget(widget, {.row: 1, .column: 2});
```

One could go even further, and mark certain fields with `std::optional`,
so that it is possible to test, if an option actually was passed:

```C++
struct DrawSomethingOptions
{
    std::optional<bool>  fillRect  = {};
    std::optional<int>   pixelSize = {};
    std::optional<qreal> pointSize = {};
};

drawSomething(..., {.pixelSize = 16});
```

This is actually pretty nice, Modern C++ has come a long way.

So only shortcoming left that even these other programming languages
with more natural approach to named arguments don't solve: One cannot
express easily, that only one of these options is valid: `fillRect`,
`pixelSize`, `pointSize`; not all of them together at the same time.
That's where this tiny library comes into play. It allows to write
the following, and only one of these options will be allowed:

```C++
struct DrawSomethingOptions : public named_options<bool, int, qreal>
{
    option<0, bool>  fillRect  = {};
    option<1, int>   pixelSize = {};
    option<2, qreal> pointSize = {};
};
```
