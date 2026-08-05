<div align="center">

# 🎐 denko

<u>minimal</u> http engine over raw tcp • sweet & lightweight

</div>

>A simple, multithreaded HTTP server built in C. Designed with zero bloat, clean file isolation, and dynamic file streaming.

<div style="
    background-color: #f6f8fa; 
    border: 1px solid #e1e4e8; 
    border-radius: 18px; 
    padding-top: 16px;
    padding-bottom: 6px;
    padding-left: 20px;
    padding-right: 20px;
    margin: 16px 0;
    ">

**Drop & Serve**  
Just drop *any* HTML, CSS, JavaScript, or media files into the `public/` directory. **denko** automatically resolves and streams them instantly, zero configuration needed.

</div>

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
│   ├── http.h      # HTTP protocol parser & MIME engine
│   └── net.h       # Socket creation & networking
├── public/         # Static assets
│   └── index.html  # Dashboard page
├── src/            # Implementation
│   ├── http.c      # HTTP request handling & static file serving
│   ├── main.c      # Main entry point
│   └── net.c       # Client connection loop & threading
├── .gitignore
├── Makefile
└── README.md