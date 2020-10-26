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
    if let Some(_) = std::env::args().skip(1).next().filter(|arg| arg == "--bs") {
        println!("{:?}", bs_test::test());
        return;
    }

    let mut proxy = Proxy::new("80.66.82.37:22005".parse().unwrap());

    if let Err(err) = proxy.startup() {
        eprintln!("startup error: {:?}", err);
        return;
    }

    let handler = handlers::hash::HashReplace::new(0xbb62c5c634026cc6);

    proxy.handlers.push(Box::new(handler));

    proxy.run();
}
