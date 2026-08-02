denko/
├── CMakeLists.txt         # Build configuration (or Makefile)
├── README.md
├── include/               # Public headers (.h files)
│   ├── denko.h            # Main server interface & config options
│   ├── http.h             # HTTP request/response structures
│   ├── net.h              # Socket creation, epoll/kqueue setup
│   └── pool.h             # Memory arena / buffer pool declarations
└── src/                   # Implementation files (.c files)
    ├── main.c             # Entry point, CLI flags, initial startup
    ├── net.c              # Non-blocking sockets, event loop listeners
    ├── http.c             # HTTP protocol parsing and formatting
    └── pool.c             # Fast custom memory allocation