# ⚡ Denko

A lightweight, high-performance C HTTP server designed around non-blocking I/O and efficient memory management.

---

## 📂 Project Structure

```text
denko/
├── CMakeLists.txt     # Build configuration
├── README.md          # Project documentation
├── include/           # Public headers
│   ├── denko.h        # Main server interface & configuration
│   ├── http.h         # HTTP request/response structures
│   ├── net.h          # Socket management & epoll/kqueue setup
│   └── pool.h         # Memory arena & buffer pool declarations
└── src/               # Implementation files
    ├── main.c         # Entry point & CLI handling
    ├── net.c          # Event loop & non-blocking I/O
    ├── http.c         # HTTP protocol parser & formatter
    └── pool.c         # Memory arena allocator