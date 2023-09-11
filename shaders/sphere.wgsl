struct VertexInput {
    // @location(0) position: vec3<f32>,
    // @location(1) normal: vec3<f32>,
    // @location(2) uv: vec2<f32>,
    @builtin(vertex_index) vertexIndex: u32
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec3<f32>,
    // position: vec4<f32>,
    // color: vec3<f32>,
};

struct Uniforms {
    mouse: vec2<f32>,
    time: f32,
};

@group(0) @binding(0)
var<uniform> uniforms: Uniforms;

@vertex
fn main(input: VertexInput) -> VertexOutput {
    var mvp1: mat3x3<f32> = mat3x3<f32>(
        vec3<f32>(1.0, 0.0, 0.0),
        vec3<f32>(0.0, cos(uniforms.time), sin(uniforms.time)),
        vec3<f32>(0.0, -sin(uniforms.time), cos(uniforms.time))
    );
    var mvp2: mat3x3<f32> = mat3x3<f32>(
        vec3<f32>(0.0, 1.0, 0.0),
        vec3<f32>(cos(uniforms.time), 0.0, -sin(uniforms.time)),
        vec3<f32>(sin(uniforms.time), 0.0, cos(uniforms.time))
    );
    var mvp3: mat3x3<f32> = mat3x3<f32>(
        vec3<f32>(cos(uniforms.time), sin(uniforms.time), 0.0),
        vec3<f32>(-sin(uniforms.time), cos(uniforms.time), 0.0),
        vec3<f32>(0.0, 0.0, 1.0)
    );
    var mvp: mat3x3<f32> = mvp1 * mvp2 * mvp3;
    var origin: vec3<f32> = vec3<f32>(0.0, 0.0, 0.0);
    var pi: f32 = 3.15159;
    var i: f32 = 1.0 * f32(input.vertexIndex);
    var r: f32 = cos(i);
    var div: f32 = 3.3;
    var theta: f32 = (i % div) * (pi) / div;
    var fi: f32 = floor(i / div) * (2.0 * pi) / div;
    var x: f32 = r * sin(theta) * cos(fi);
    var y: f32 = r * sin(theta) * sin(fi);
    var z: f32 = r * cos(theta);
    var output: VertexOutput;
    output.color = (vec3<f32>(x, y, z)) * cos(uniforms.time);
    var pos: vec3<f32> = (vec3<f32>(origin.x + x, origin.y + y, origin.z + z) * mvp);
    // return vec4<f32>(pos, 1.0);
    output.position = vec4<f32>(pos, 1.0);
    return output;
}

// @fragment
// fn fmain(in: VertexOutput) -> @location(0) vec4<f32> {
//     // return vec4<f32>(in.color,1.0);
//     return in.position;
// }