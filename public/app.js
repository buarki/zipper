const KB = 1024;
const MB = 1024 * KB;
const MAX_ALLOWED_SIZE = 150 * MB; // We compile wasm allowing 200MB, so allowing 150MB is ok :)
const COMPRESSED_FILE_MAX_ALLOWED_SIZE = MAX_ALLOWED_SIZE * 1.3; // A little bit bigger to support the scenario of ineffective compression.
const MILLISECONDS = 1;
const SECOND = 1000 * MILLISECONDS;
const ZIPPER_COMPRESSED_FILE_EXTENSION = '.zipp';
const COMPRESSED_FILE_NAME = `compressed_file${ZIPPER_COMPRESSED_FILE_EXTENSION}`;
const DECOMPRESSED_FILE_NAME = `your_decompressed_file`;

const LOADING = (color = 'border-blue-500') =>
        `<div class="animate-spin h-5 w-5 border-t-4 ${color} border-solid rounded-full"></div>`;

function validateFileSize(inputId, allowedFileSize, onValidFileReceived) {
  const input = document.getElementById(inputId);
  if (input.files && input.files[0]) {
    if (input.files[0].size > allowedFileSize) {
      alert("File size exceeds the maximum allowed one (150MB). Please choose a smaller file.");
      input.value = '';
      return;
    }
  }
  onValidFileReceived();
}

function validateFileToCompress(inputId) {
  const onValidFileToCompress = () => {
    const compressButton = document.getElementById('compressButton');
    compressButton.classList.add('bg-blue-500');
    compressButton.classList.add('hover:bg-blue-300');
    compressButton.disabled = false;
  };
  validateFileSize(inputId, MAX_ALLOWED_SIZE, onValidFileToCompress);
}

function validateFileToDecompress(inputId) {
  const onValidFileToDecompress = () => {
    const decompressButton = document.getElementById('decompressButton');
    decompressButton.classList.add('bg-green-500');
    decompressButton.classList.add('hover:bg-green-300');
    decompressButton.disabled = false;
  };
  validateFileSize(inputId, COMPRESSED_FILE_MAX_ALLOWED_SIZE, onValidFileToDecompress);
}

async function compress(file) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = function (event) {
      const fileData = new Uint8Array(event.target.result);

      const fileContentPtr = Module._malloc(fileData.length);

      Module.writeArrayToMemory(fileData, fileContentPtr);

      const compressedContentSize = Module.ccall(
                        'c_compress',
                        'number',
                        ['[number]', 'number'],
                        [fileContentPtr, fileData.length]);

      const buffer = Module._malloc(compressedContentSize);
      Module.ccall(
        'receiveContent',
        null,
        ['[number]', 'number'],
        [buffer,compressedContentSize],
      );
      const compressedContent = new Uint8Array(Module.HEAPU8.buffer).slice(buffer, buffer + compressedContentSize);

      _free(fileContentPtr);
      _free(buffer);
     
      resolve({ compressedContent, compressedContentSize, originalSize: fileData.length });
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

      const decompressedSize = Module.ccall(
                        'c_decompress',
                        'number',
                        ['[number]', 'number'],
                        [fileContentPtr, fileData.length]);

      const buffer = Module._malloc(decompressedSize);
      Module.ccall(
        'collectDecompressedContent',
        null,
        ['[number]', 'number'],
        [buffer,decompressedSize],
      );
      const decompressedContent = new Uint8Array(Module.HEAPU8.buffer).slice(buffer, buffer + decompressedSize);

      _free(fileContentPtr);
      _free(buffer);
     
      resolve({ decompressedContent, decompressedSize });
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
  const feedbackCard = document.getElementById('notificationCard');
  const fileInput = document.getElementById('fileInput');
  const compressButton = document.getElementById('compressButton');
  const file = fileInput.files[0];

  if (file) {
    compressButton.disabled = true;
    compressButton.innerHTML = LOADING('bg-blue-500');
    try {
      const { compressedContent, compressedContentSize, originalSize } = await compress(file);
      downloadFile(compressedContent, COMPRESSED_FILE_NAME, 'application/octet-stream');
      const { feedbackMessage, textColor } = getCompressionFeedback(originalSize, compressedContentSize);
      showNotificationCard(feedbackMessage, textColor, feedbackCard);

      compressButton.classList.remove('bg-blue-500');
      compressButton.classList.remove('hover:bg-blue-300');
      compressButton.classList.add('bg-blue-200');
      compressButton.disabled = true;

      fileInput.value = '';
    } catch (error) {
      console.error('Error getting file content:', error);
    } finally {
      compressButton.innerHTML = 'Compress';
      compressButton.disabled = false;
    }
  } else {
    alert('Please select a file.');
  }
}

async function uploadFileToDecompress() {
  const compressedFileInput = document.getElementById('decompressFileInput');
  const decompressButton = document.getElementById('decompressButton');
  const compressedFile = compressedFileInput.files[0];
  if (compressedFile) {
    decompressButton.disabled = true;
    decompressButton.innerHTML = LOADING('bg-green-500');
    try {
      const { decompressedContent } = await decompress(compressedFile);
      downloadFile(decompressedContent, DECOMPRESSED_FILE_NAME, 'application/octet-stream');

      decompressButton.classList.remove('bg-green-500');
      decompressButton.classList.remove('hover:bg-green-300');
      decompressButton.classList.add('bg-green-200');
      decompressButton.disabled = true;

      compressedFileInput.value = '';
    } catch (error) {
      console.error('failed to decompress:', error);
    } finally {
      decompressButton.disabled = false;
      decompressButton.innerHTML = `Decompress`;
    }
  } else {
    alert('Please select a file to decompress.');
  }
}

Module.onRuntimeInitialized = function() {}

function getCompressionFeedback(originalSize, compressedSize) {
  const compressionWasEffective = compressedSize < originalSize;
  return {
    feedbackMessage: compressionWasEffective ? `Compressed file is ${((1 - (compressedSize / originalSize)) * 100).toFixed(2)}% smaller :)` : `Compressed file is bigger than original ;-;`,  
    textColor: compressionWasEffective ? 'bg-green-500' : 'bg-yellow-500',
  };
}

function showNotificationCard(message, textColor, cardElement) {
  const timeVisible = 7 * SECOND;

  cardElement.innerText = message;
  cardElement.classList.add(textColor);
  cardElement.style.display = 'block';
  cardElement.offsetWidth;
  cardElement.style.opacity = 1;

  setTimeout(() => {
    cardElement.classList.remove(textColor);
    cardElement.style.display = 'none';
  }, timeVisible);
}
