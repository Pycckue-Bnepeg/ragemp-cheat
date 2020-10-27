#![feature(abi_thiscall, proc_macro_hygiene, decl_macro)]
#![allow(
    dead_code,
    non_snake_case,
    non_camel_case_types,
    non_upper_case_globals
)]

#[macro_use]
extern crate rocket;

mod bs_test;
pub mod handler;
pub mod handlers;
pub mod rage;
pub mod raknet;

mod proxy;

use proxy::Proxy;

fn main() {
    use std::hash::Hasher;
    use std::io::Write;

    // 4737987 (hex 484bc3) 48:4b:c3
    if let Some(_) = std::env::args().skip(1).next().filter(|arg| arg == "--bs") {
        let len = std::fs::metadata("E:\\games\\index.js").unwrap().len();
        let mut hasher = twox_hash::XxHash64::with_seed(0);
        hasher.write(&len.to_le_bytes());
        let hash = hasher.finish();
        println!("len: {} hash: {:16x} bytes {:16x}", len, hash, len);
        // println!("{:?}", bs_test::test());
        return;
    }

    let mut proxy = Proxy::new("80.66.82.37:22005".parse().unwrap());

    if let Err(err) = proxy.startup() {
        eprintln!("startup error: {:?}", err);
        return;
    }

    let handler = handlers::hash::HashReplace::new(0xf33cf41fdcc5c61c);

    proxy.handlers.push(Box::new(handler));

    proxy.run();
}
