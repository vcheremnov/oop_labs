#pragma once

class Window {
public:
    // type names
    using pos = int;
    using size_type = int;
    // constructors
    virtual ~Window() = default;
    // public methods
    virtual void clear() = 0;
    virtual void refresh() = 0;
    virtual size_type get_width() = 0;
    virtual size_type get_height() = 0;
};
