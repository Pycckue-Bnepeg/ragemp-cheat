#![feature(proc_macro_hygiene, decl_macro)]
#[macro_use] extern crate rocket;

use rocket::config::{Config, Environment};
use rocket::response::{Redirect, Response};
use rocket::http::Header;
use rocket::State;

use std::fs::File;
use std::io::{Write, Cursor};
use std::path::Path;
use std::sync::{Arc, Mutex};

use twox_hash::XxHash;

mod manipulator;

use crate::manipulator::Manipulator;

type ManState<'a> = State<'a, Arc<Mutex<Manipulator>>>;

#[get("/")]
fn list(man: ManState) -> Option<Response<'static>> {
    let mut req_response = reqwest::get("http://145.239.149.95:22006/list/").ok()?;
    let mut cursor = Cursor::new(vec![]);

    req_response.copy_to(&mut cursor).ok()?;

    let mut man = man.lock().unwrap();
    man.generate(&mut cursor);

    let response = Response::build()
        .header(Header::new("Content-Type", "text/html"))
        .sized_body(cursor).finalize();
    
    Some(response)
}

#[get("/file/<index>")]
fn file(man: ManState, index: usize) -> Option<Response<'static>> {
    let man = man.lock().unwrap();
    let mut cursor = Cursor::new(vec![]);

    if man.is_origin_script_index(index) {
        fetch_and_write(man.origin_script_index(), &mut cursor)?;
    } else if man.is_custom_script_index(index) {
        open_and_write(man.script_path(), &mut cursor)?;
    } else {
        fetch_and_write(index, &mut cursor)?;
    }

    let response = Response::build()
        .header(Header::new("Content-Type", "text/html"))
        .sized_body(cursor).finalize();
    
    Some(response)
}

fn open_and_write(path: &Path, cursor: &mut Cursor<Vec<u8>>) -> Option<()> {
    let mut file = File::open(path).ok()?;
    std::io::copy(&mut file, cursor).ok()?;
    Some(())
}

fn fetch_and_write(index: usize, cursor: &mut Cursor<Vec<u8>>) -> Option<()> {
    let mut req_response = reqwest::get(&format!("http://145.239.149.95:22006/file/{}", index)).ok()?;
    req_response.copy_to(cursor).ok()?;
    Some(())
}

fn main() {
    use std::hash::Hasher;

    // генерация хеша при старте
    let mut man = Manipulator::new("./js-scripts/index.js");

    let mut req_response = reqwest::get("http://145.239.149.95:22006/list/").expect("remote server is dead");
    let mut cursor = Cursor::new(vec![]);

    req_response.copy_to(&mut cursor).unwrap();
    man.generate(&mut cursor);

    let mut hasher = XxHash::with_seed(0);
    hasher.write(cursor.get_ref());
    let hash = hasher.finish();

    println!("hash: {:16x}", hash);


    let man = Arc::new(Mutex::new(man));
    let config = Config::build(Environment::Staging).port(22006).finalize().unwrap();
    
    rocket::custom(config)
        .manage(man)
        .mount("/", routes![file])
        .mount("/list", routes![list])
        .launch();
}
