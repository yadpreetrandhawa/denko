# 🎐 denko

minimal http engine over raw tcp • sweet & lightweight

A simple, multithreaded HTTP server built in C. Designed with zero bloat, clean file isolation, and dynamic file streaming.

## Install

#### Dependencies
* clang
* make

### Download
```bash
git clone https://github.com/yadpreetrandhawa/denko
cd denko
make
```

## run
```bash
./bin/denko
# denko can be added to your corresponding shell path
```

#### To exit denko, press [CTRL + C]

## File Structure

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