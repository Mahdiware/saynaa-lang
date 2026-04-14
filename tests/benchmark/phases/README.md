# Benchmark Phase Scripts

Each `.sa` file in this folder targets a specific performance phase.

- compile_frontend.sa: parser/compiler workload
- runtime_loop.sa: arithmetic loop baseline
- runtime_function_call.sa: function call overhead
- runtime_method_dispatch.sa: method dispatch overhead
- runtime_attribute_access.sa: attribute get/set overhead
- runtime_collections.sa: list/map write/read workload
- runtime_string_ops.sa: string split/join/transform workload
- module_import.sa: module call path workload

`modules/` contains helper modules used by `module_import.sa`.
