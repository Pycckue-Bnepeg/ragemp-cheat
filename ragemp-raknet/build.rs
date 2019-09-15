fn main() {
//    let cat = std::env::var("BIG_x64_PATH").expect("no big_x64 path");
    let cat = std::env::var("BIG_x64_PATH").unwrap_or(String::from("D:\\sources\\projects\\ragemp-cheat\\ragemp-raknet\\RakNet\\DependentExtensions\\cat\\lib\\cat"));
    let out = cmake::Config::new("RakNet").no_build_target(true).build();

    // bindgen wrappers.h -o rakpeer.rs --whitelist-type RakNet::RakPeerInterface --whitelist-type RakNet::BitStream --whitelist-type OutOfBandIdentifiers --whitelist-type DefaultMessageIDTypes --no-layout-tests --enable-cxx-namespaces --no-derive-copy --rust-target nightly --default-enum-style moduleconsts -- -x c++ --std=c++14
    // let bindings = bindgen::builder()
    //     .header("./RakNet/Source/RakPeerInterface.h")
    //     .header("./RakNet/Source/BitStream.h")
    //     .whitelist_type("RakNet::RakPeerInterface")
    //     .whitelist_type("RakNet::BitStream")
    //     .derive_copy(false)
    //     .layout_tests(false)
    //     .enable_cxx_namespaces()
    //     .rust_target(bindgen::RustTarget::Nightly)
    //     .default_enum_style(bindgen::EnumVariation::ModuleConsts)
    //     .clang_args(&["-xc++", "--std=c++14"])
    //     .rustfmt_bindings(true)
    //     .generate()
    //     .expect("couldn't generate bindings");

    // bindings.write_to_file("raknet.rs");

    // bindgen 1 -o 1 --whitelist-type SAMP::CNetGame --no-layout-tests --enable-cxx-namespaces --no-derive-copy --rust-target nightly --default-enum-style moduleconsts -- -x c++ --std=c++17

    // todo: lib path
    let mut profile = std::env::var("PROFILE").unwrap();
//    profile.chars().nth(0).unwrap() = profile[0].to_uppercase();

    println!(
        "cargo:rustc-link-search=native={}/build/Lib/LibStatic/{}",
        out.display(),
        profile,
    );

    println!("cargo:rustc-link-lib=static=RakNetLibStatic");

    println!("cargo:rustc-link-search=native={}", cat);
    println!("cargo:rustc-link-lib=static=big_x64");

    // todo: if windows
    println!("cargo:rustc-link-lib=static=User32");
}
