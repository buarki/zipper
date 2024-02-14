const KB = 1024;
const MAX_ALLOWED_SIZE = 40 * KB;

function validateFileSize(inputId) {
  const input = document.getElementById(inputId);
  if (input.files && input.files[0]) {
    if (input.files[0].size > MAX_ALLOWED_SIZE) {
      alert("File size exceeds the maximum allowed one (40KB). Please choose a smaller file.");
      input.value = '';
      return;
    }
  }
}

async function compress(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = function (event) {
      const fileData = new Uint8Array(event.target.result);

      const fileContentPtr = Module._malloc(fileData.length);

      Module.writeArrayToMemory(fileData, fileContentPtr);
      console.log(`writeArrayToMemory [${fileData.length}]`);

      const size = Module.ccall(
                        'c_compress',
                        'number',
                        ['[number]', 'number'],
                        [fileContentPtr, fileData.length]);
      console.log({size});

      const buffer = Module._malloc(size);
      Module.ccall(
        'receiveContent',
        null,
        ['[number]', 'number'],
        [buffer,size],
      );
      const content = new Uint8Array(Module.HEAPU8.buffer).slice(buffer, buffer + size);

      _free(fileContentPtr);
      _free(buffer);
     
      resolve(content);
    };

    reader.onerror = function (event) {
      reject(event.target.error);
    };

    reader.readAsArrayBuffer(file);
  });
}

async function decompress(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = function (event) {
      const fileData = new Uint8Array(event.target.result);

      const fileContentPtr = Module._malloc(fileData.length);

      Module.writeArrayToMemory(fileData, fileContentPtr);
      console.log(`writeArrayToMemory [${fileData.length}]`);

      const size = Module.ccall(
                        'c_decompress',
                        'number',
                        ['[number]', 'number'],
                        [fileContentPtr, fileData.length]);
      console.log({size});

      const buffer = Module._malloc(size);
      Module.ccall(
        'collectDecompressedContent',
        null,
        ['[number]', 'number'],
        [buffer,size],
      );
      const content = new Uint8Array(Module.HEAPU8.buffer).slice(buffer, buffer + size);

      _free(fileContentPtr);
      _free(buffer);
     
      resolve(content);
    };

    reader.onerror = function (event) {
      reject(event.target.error);
    };

    reader.readAsArrayBuffer(file);
  });
}

function downloadFile(data, filename, type) {
  const blob = new Blob([data], { type: type });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  URL.revokeObjectURL(url);
}

async function uploadFileToCompress() {
  const fileInput = document.getElementById('fileInput');
  const file = fileInput.files[0];

  if (file) {
    try {
      const fileContent = await compress(file);
      downloadFile(fileContent, 'compressed_file.zipp', 'application/octet-stream');
    } catch (error) {
      console.error('Error getting file content:', error);
    }
  } else {
    alert('Please select a file.');
  }
}

async function uploadFileToDecompress() {
  const compressedFileInput = document.getElementById('decompressFileInput');
  const compressedFile = compressedFileInput.files[0];
  if (compressedFile) {
    try {
      const decompressedFileContent = await decompress(compressedFile);
      downloadFile(decompressedFileContent, 'your_decompressed_file', 'application/octet-stream');
    } catch (error) {
      console.error('failed to decompress:', error);
    }
  } else {
    alert('Please select a file to decompress.');
  }
}

Module.onRuntimeInitialized = function() {
  console.log('initialized!');
}
