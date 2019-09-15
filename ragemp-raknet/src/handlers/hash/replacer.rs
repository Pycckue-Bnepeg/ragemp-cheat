pub struct Replacer {
    origin_host: String,
}

impl Replacer {
    pub fn new(origin_host: String) -> Replacer {
        Replacer {
            origin_host,
        }
    }

    pub fn origin_host(&self) -> &str {
        &self.origin_host
    }
}
