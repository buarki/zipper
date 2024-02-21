# zipper

## I just want to use it

No prob, visit the project deployed on vercel: https://zipper-zeta.vercel.app.

## What is this project?

It is simple file compressor written in C. It is based on the [Huffman Coding Algorithm](https://en.wikipedia.org/wiki/Huffman_coding), which is a lossless and very efficient compression algorithm.

If you want you to see a full description and overview of this project you can [read my article explaining it in details](https://www.buarki.com/blog/wasm-huffman).

## Developing

### Install Emscripten

Just follow its [documentation](https://emscripten.org/docs/getting_started/downloads.html), this one really works :)

### Activate EMscripten

```sh
source "/path/to/emsdk/emsdk_env.sh"
```

### Build the C binary

```sh
make wasm
```

### Deploying the local server

```sh
make emserve
```

### Running Unit tests

```sh
make test
```

## Deployment

This project is [deployed at Vercel](https://vercel.com/). It basically makes the content present on [public](/public/) directory available to the web. 
