#![feature(abi_thiscall, proc_macro_hygiene, decl_macro)]
#![allow(
    dead_code,
    non_snake_case,
    non_camel_case_types,
    non_upper_case_globals
)]

#[macro_use]
extern crate rocket;

pub mod raknet;
pub mod rage;
pub mod handler;
pub mod handlers;

mod proxy;

use proxy::Proxy;

fn main() {
    let proxy = Proxy::new("127.0.0.1:22009".parse().unwrap());
    
    if let Err(err) = proxy.startup() {
        eprintln!("startup error: {:?}", err);
        return;
    }

    proxy.run();
}
