# Zen Web API Framework (C++)

`Zen` is a compact C++ **web/API framework** built for **TCP-based HTTP request/response** workflows, with a strong focus on **middleware-driven routing**, **content negotiation**, and **concurrent connection handling**.

## IMPLEMENTED FEATURES (Core Strengths)

### 1) **L4 Transport Layer Load Balancing + Multi-Threading**

At the **Layer 4 (TCP)** boundary, `Zen::listen(...)` continuously accepts incoming client sockets and dispatches each accepted connection to a worker task in a **`ThreadPool`**.

- Uses **`std::thread::hardware_concurrency()`** to size the worker pool according to available cores.
- Decouples accept-loop socket acquisition from request handling execution.
- Enables practical **concurrent request servicing** across many client connections.
- Delivers a clean **transport-level load distribution** pattern for the framework runtime.

### 2) **Asynchronous Per-Client Handling via Move-Only `ClientSession`**

`ClientSession` is implemented from scratch to provide strict connection ownership and safe task transfer.

- Implements **move constructor** and **move assignment**.
- Explicitly deletes **copy constructor** and **copy assignment**.
- Enforces **single-owner socket semantics**, preventing accidental duplication of live connection handles.
- Integrates directly with thread-pool task capture (`cs = std::move(cs)`) for robust asynchronous workflow.

This design is especially valuable for server correctness under heavy concurrent traffic.

### 3) **Content Negotiation with `Accept` Header Priority Processing**

The framework supports response representation selection based on client preferences:

- `Response::format("Accept", handler_map)` reads the client `Accept` header.
- `ContentNegotiator::parse(...)` converts accepted media values into **`FormatPriority`** (`std::priority_queue`).
- `ContentNegotiator::match(...)` selects the best representation from registered handlers.
- Automatically sets `Content-Type` and executes the selected formatter callback.
- Falls back to `406 Not Acceptable` when no representation can satisfy the client request.

This gives `Zen` a technically strong foundation for **multi-format APIs**.

### 4) **Middleware Pipeline Architecture (DEFAULT / PATH / ERR)**

`Zen` processes requests through a pipeline that supports three middleware categories:

- **DEFAULT middleware** for generic request/response cross-cutting logic.
- **PATH middleware** for method + route handler dispatch.
- **ERR middleware** for centralized application-level error handling (`ZenErr` flow).

Additional technical strengths:

- Middleware is processed in deterministic insertion order.
- `PathMiddleware` supports route parameters (`:id`, `:slug`, etc.) and extracts them into `req.params`.
- Query components are parsed and attached to `req.query` on route match.
- Pipeline behavior remains explicit and testable.

### 5) **Fluent `http::Response` Function Chaining**

The response API is designed for concise and expressive handler code:

- Chainable methods return `Response&`: `status(...)`, `send(...)`, `json(...)`, `header(...)`, `sendFile(...)`.
- `construct()` serializes the final **HTTP/1.1** response string (status line + headers + body).
- Supports clean response assembly while preserving low-level control.

This function-chaining style improves handler ergonomics without hiding protocol-level details.  
<br>

# Getting Started

Usage-first documentation for building APIs with `Zen` (similar style to Express usage docs).

## Installation / Include

```cpp
#include <zen/app/app.hpp>
```

## Create an app

```cpp
Zen app;
```

## Start the server

```cpp
app.listen(8080, []() {
    std::cout << "Server running on port 8080\n";
});
```

## Register middleware

Use `app.use(...)` with a callback to run logic for incoming requests.

```cpp
app.use([](http::Request& req, http::Response& res) {
    std::cout << req.method << " " << req.path << std::endl;
});
```

## Register routes

Use method + path routing with:

```cpp
app.use("GET", "/users/:id", [](http::Request& req, http::Response& res) {
    res.send("user route");
});
```

### Route parameters

Path params use `:name` and are available in `req.params`.

```cpp
app.use("GET", "/users/:id", [](http::Request& req, http::Response& res) {
    std::string id = req.params["id"];
    res.json("{\"id\":\"" + id + "\"}");
});
```

### Query parameters

Query values are available in `req.query`.

```cpp
app.use("GET", "/search", [](http::Request& req, http::Response& res) {
    // Example request: /search?q=zen
    auto q = req.query["q"];
    if (!q.empty()) {
        res.send("Query: " + q[0]);
        return;
    }
    res.status(400).send("Missing q");
});
```

## Request object

Inside handlers, use:

- `req.method`
- `req.path`
- `req.version`
- `req.headers`
- `req.body`
- `req.params`
- `req.query`

## Response object

`Response` supports function chaining:

```cpp
app.use("GET", "/health", [](http::Request& req, http::Response& res) {
    res.status(200).header(std::pair<std::string, std::string>{"X-App", "Zen"}).send("OK");
});
```

Common response helpers:

- `res.send("text")`
- `res.json("{\"ok\":true}")`
- `res.status(201).send("created")`
- `res.sendFile("public/index.html")`
- `res.redirect("https://example.com")`

## Content negotiation

Use `res.format(...)` to return different output types based on `Accept`.

```cpp
app.use("GET", "/profile/:id", [](http::Request& req, http::Response& res) {
    auto id = req.params["id"];

    res.format("Accept", {
        {"application/json", [&]() {
            res.json("{\"id\":\"" + id + "\"}");
        }},
        {"text/html", [&]() {
            res.send("<h1>User " + id + "</h1>");
        }}
    });
});
```

## Error middleware

Use error middleware with:

```cpp
app.use([](const ZenErr& e, http::Request& req, http::Response& res) {
    res.status(500).send(e.what());
});
```

Inside route/default middleware, throw `ZenErr` for app-level errors:

```cpp
app.use("GET", "/fail", [](http::Request& req, http::Response& res) {
    throw ZenErr("Something went wrong", true);
});
```

## Complete example

```cpp
#include <iostream>
#include <zen/app/app.hpp>

int main() {
    Zen app;

    app.use([](http::Request& req, http::Response& res) {
        std::cout << req.method << " " << req.path << std::endl;
    });

    app.use("GET", "/", [](http::Request& req, http::Response& res) {
        res.send("Welcome to Zen");
    });

    app.use("GET", "/users/:id", [](http::Request& req, http::Response& res) {
        auto id = req.params["id"];
        res.format("Accept", {
            {"application/json", [&]() { res.json("{\"id\":\"" + id + "\"}"); }},
            {"text/html", [&]() { res.send("<h1>User " + id + "</h1>"); }}
        });
    });

    app.use([](const ZenErr& e, http::Request& req, http::Response& res) {
        res.status(500).send(e.what());
    });

    app.listen(8080, []() {
        std::cout << "Listening on 8080\n";
    });
}
```

## Suggested Improvements (Next Steps)

- **Radix tree routing** for faster route-handler lookup as route volume grows.
- **Manual response commit** instead of full auto-commit flow after middleware execution.
- **Cookie support** (`Cookie` parsing and `Set-Cookie` response helpers).
- **WebSocket support** (`Upgrade` handshake + frame processing loop).
- **Explicit JSON parsing** with typed request-body decoding utilities.
- **Database programming interfaces** for cleaner data-access abstraction.
- **Exception model refinement**:
  - Current explicit `std::runtime_error` cases:
    - `ClientSession::recvFromClient()` -> `"Request rejected, too large."`, `"Request Timeout"`
    - `ClientSession::sendToClient()` -> `"Retry fail"`
  - `http::Request` already maps invalid request parsing to `HttpErr(400, ...)`.
  - `ContentNegotiator::parse(...)` may emit standard conversion failures (`std::invalid_argument`/`std::out_of_range`) via `std::stof(...)`.
  - Add a broader `std::exception` catch boundary and route specific argument/parsing cases to `std::invalid_argument`, while reserving `std::logic_error` for internal invariants.