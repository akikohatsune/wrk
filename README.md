# wrk - a HTTP benchmarking tool (Modified Fork)

  wrk is a modern HTTP benchmarking tool capable of generating significant
  load when run on a single multi-core CPU. It combines a multithreaded
  design with scalable event notification systems such as epoll and kqueue.

  This version is a **modified fork** that adds several features and infrastructure
  improvements over the original wg/wrk.
## New Features & Enhancements

  * **JSON Output:** Support for exporting benchmark results directly to JSON format.
  * **Live P99 Latency:** Real-time reporting of P99 latency statistics.
  * **Staggered Connections:** Support for "warmup" periods via staggered connection establishment.
  * **Integrated `lua-cjson`:** Full integration of the cjson library for faster and more flexible JSON handling within Lua scripts.
  * **Warmup Support:** Logic for gradual load increase to avoid overwhelming servers during initial spikes.

## Usage of New Features

### JSON Output
To get results in a machine-readable JSON format, use the `-J` or `--json` flag:
```bash
wrk -t12 -c400 -d30s -J http://127.0.0.1:8080/
```
The output will include detailed request stats, latency percentiles (p50, p75, p90, p99, p999), and error counts.

### Warmup (Staggered Connections)
To avoid a massive burst of connections at the very start, you can stagger connection establishment over a warmup period using the `-w` or `--warmup` flag:
```bash
wrk -t12 -c400 -d30s -w 10s http://127.0.0.1:8080/
```
In this example, the 400 connections will be opened gradually over the first 10 seconds of the test.

### Live P99 Latency Reporting
When running a standard test (without `--json`), `wrk` now provides real-time progress updates every second, including the current P99 latency:
```
  Progress:   1s,   56200.00 req/s,    606.33MB/s, p99: 850.12us
  Progress:   2s,   56500.00 req/s,    608.12MB/s, p99: 865.40us
```

### Lua CJSON
You can now use the `cjson` module directly within your Lua scripts for high-performance JSON encoding and decoding:
```lua
local cjson = require "cjson"
function request()
    local body = cjson.encode({ hello = "world" })
    return wrk.format("POST", "/", nil, body)
end
```

## Basic Usage

## Build & CI/CD Improvements

  * **Universal ARM64 Support:** Robust build process for ARM64 (Android/Termux/iOS) using QEMU-emulated Docker environments.
  * **Expanded OS Support:** Automated CI builds for Ubuntu, macOS, Arch Linux, and Fedora (RPM).
  * **Modern Toolchain:** Explicit support for Node.js 24 and Ubuntu 22.04 environments.
  * **Deterministic Dependencies:** Bundled source/archives for LuaJIT and OpenSSL in `deps/` to ensure consistent builds across different systems.

## Basic Usage

    wrk -t12 -c400 -d30s http://127.0.0.1:8080/index.html

  This runs a benchmark for 30 seconds, using 12 threads, and keeping
  400 HTTP connections open.

  Output:

    Running 30s test @ http://127.0.0.1:8080/index.html
      12 threads and 400 connections
      Thread Stats   Avg      Stdev     Max   +/- Stdev
        Latency   635.91us    0.89ms  12.92ms   93.69%
        Req/Sec    56.20k     8.07k   62.00k    86.54%
      22464657 requests in 30.00s, 17.76GB read
    Requests/sec: 748868.53
    Transfer/sec:    606.33MB

## Command Line Options

    -c, --connections: total number of HTTP connections to keep open with
                       each thread handling N = connections/threads

    -d, --duration:    duration of the test, e.g. 2s, 2m, 2h

    -t, --threads:     total number of threads to use

    -s, --script:      LuaJIT script, see SCRIPTING

    -H, --header:      HTTP header to add to request, e.g. "User-Agent: wrk"

        --latency:     print detailed latency statistics

        --timeout:     record a timeout if a response is not received within
                       this amount of time.

## Benchmarking Tips

  The machine running wrk must have a sufficient number of ephemeral ports
  available and closed sockets should be recycled quickly. To handle the
  initial connection burst the server's listen(2) backlog should be greater
  than the number of concurrent connections being tested.

  A user script that only changes the HTTP method, path, adds headers or
  a body, will have no performance impact. Per-request actions, particularly
  building a new HTTP request, and use of response() will necessarily reduce
  the amount of load that can be generated.

## Acknowledgements

  wrk contains code from a number of open source projects including the
  'ae' event loop from redis, the nginx/joyent/node.js 'http-parser',
  and Mike Pall's LuaJIT. Please consult the NOTICE file for licensing
  details.

## Cryptography Notice

  This distribution includes cryptographic software. The country in
  which you currently reside may have restrictions on the import,
  possession, use, and/or re-export to another country, of encryption
  software. BEFORE using any encryption software, please check your
  country's laws, regulations and policies concerning the import,
  possession, or use, and re-export of encryption software, to see if
  this is permitted. See <http://www.wassenaar.org/> for more
  information.

  The U.S. Government Department of Commerce, Bureau of Industry and
  Security (BIS), has classified this software as Export Commodity
  Control Number (ECCN) 5D002.C.1, which includes information security
  software using or performing cryptographic functions with symmetric
  algorithms. The form and manner of this distribution makes it
  eligible for export under the License Exception ENC Technology
  Software Unrestricted (TSU) exception (see the BIS Export
  Administration Regulations, Section 740.13) for both object code and
  source code.

## Recent Changes

The following updates were made to the CI/CD pipeline and project configuration:

1.  **Fixed ARM64 Build:** Replaced the direct cross-compilation approach with a QEMU-emulated Docker build (`ubuntu:22.04` on `linux/arm64`) to ensure all dependencies (LuaJIT, OpenSSL) are correctly compiled.
2.  **Updated Runner Environments:**
    *   Main runners pinned to `ubuntu-22.04`.
    *   Explicitly configured **Node.js 24** across all jobs using `actions/setup-node@v4`.
    *   Fedora build pinned to **Fedora 36**.
    *   Arch Linux build updated to use latest packages with full sync (`pacman -Syyu`).
3.  **CI Optimization:** Reorganized job steps to ensure code checkout occurs before dependency installation in containerized environments.
