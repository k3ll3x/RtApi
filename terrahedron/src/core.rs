pub mod core {
    use wasm_bindgen::prelude::*;

    #[wasm_bindgen]
    pub struct Vecf32 {
        xs: Vec<f32>,
    }
    
    #[wasm_bindgen]
    impl Vecf32 {
        #[wasm_bindgen(constructor)]
        pub fn new() -> Vecf32 {
            Vecf32 { xs: vec![] }
        }

        #[wasm_bindgen]
        pub fn push(&mut self, n: f32) {
            self.xs.push(n);
        }

        #[wasm_bindgen]
        pub fn each(&self, f: &js_sys::Function) {
            let this = JsValue::null();
            for &x in &self.xs {
                let x = JsValue::from(x);
                let _ = f.call1(&this, &x);
            }
        }
    }
}