[[block]]
struct CellBuffer {
    cells: array<u32>;
};

[[block]]
struct OutputBuffer {
    cells: array<u32>;
};

[[group(0), binding(0)]]
var<storage, read> cell_buffer: CellBuffer;

[[group(0), binding(1)]]
var<storage, write> output_buffer: OutputBuffer;

[[stage(compute), workgroup_size(32, 32, 1)]]
fn main([[builtin(global_invocation_id)]] id: vec3<u32>) {
    let index = id.x + id.y * size.x;
    let cell = cell_buffer.cells[index];
    let neighbors = cell_buffer.cells[(index + size.x - 1) % size.x + (index / size.x + size.y - 1) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x) % size.x + (index / size.x + size.y - 1) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x + 1) % size.x + (index / size.x + size.y - 1) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x - 1) % size.x + (index / size.x) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x + 1) % size.x + (index / size.x) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x - 1) % size.x + (index / size.x + 1) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x) % size.x + (index / size.x + 1) % size.y * size.x]
                  + cell_buffer.cells[(index + size.x + 1) % size.x + (index / size.x + 1) % size.y * size.x];
    if cell == 1 {
        if neighbors < 2 || neighbors > 3 {
            output_buffer.cells[index] = 0;
        } else {
            output_buffer.cells[index] = 1;
        }
    } else {
        if neighbors == 3 {
            output_buffer.cells[index] = 1;
        } else {
            output_buffer.cells[index] = 0;
        }
    }
}