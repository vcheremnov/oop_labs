#pragma once

class Window {
public:
    // type names
    using pos = long;
    using size_type = long;
    // constructors
    virtual ~Window() = default;
    // public methods
    virtual void clear() = 0;
    virtual void refresh() = 0;
    virtual size_type get_width() = 0;
    virtual size_type get_height() = 0;
};
