# 🎐 denko

minimal http engine • sweet & lightweight

A simple, multithreaded HTTP server built in C. Designed with zero bloat, clean file isolation, and dynamic file streaming.


## 📂 Structure

```text
denko/
├── bin/            # Compiled binaries
├── include/        # Header files
│   ├── denko.h     # Core server declarations
│   ├── http.h      # HTTP protocol parser & MIME engine
│   ├── net.h       # Socket creation & networking
│   └── pool.h      # Thread pool declarations
├── public/         # Static assets
│   └── index.html  # Dashboard page
├── src/            # Implementation
│   ├── http.c      # HTTP request handling & static file serving
│   ├── main.c      # Main entry point
│   ├── net.c       # Client connection loop & threading
│   └── pool.c      # Thread pool worker queue
├── .gitignore
├── Makefile
└── README.md