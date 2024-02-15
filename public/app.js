const KB = 1024;
const MAX_ALLOWED_SIZE = 40 * KB;
const MILLISECONDS = 1;
const SECOND = 1000 * MILLISECONDS;
const ZIPPER_COMPRESSED_FILE_EXTENSION = '.zipp';
const COMPRESSED_FILE_NAME = `compressed_file${ZIPPER_COMPRESSED_FILE_EXTENSION}`;
const DECOMPRESSED_FILE_NAME = `your_decompressed_file`;

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
  const file = fileInput.files[0];

  if (file) {
    try {
      const { compressedContent, compressedContentSize, originalSize } = await compress(file);
      downloadFile(compressedContent, COMPRESSED_FILE_NAME, 'application/octet-stream');
      const { feedbackMessage, textColor } = getCompressionFeedback(originalSize, compressedContentSize);
      showNotificationCard(feedbackMessage, textColor, feedbackCard);
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
      const { decompressedContent } = await decompress(compressedFile);
      downloadFile(decompressedContent, DECOMPRESSED_FILE_NAME, 'application/octet-stream');
    } catch (error) {
      console.error('failed to decompress:', error);
    }
  } else {
    alert('Please select a file to decompress.');
  }
}

Module.onRuntimeInitialized = function() {}

function getCompressionFeedback(originalSize, compressedSize) {
  const compressionWasEffective = compressedSize < originalSize;
  return {
    feedbackMessage: `Original Size: ${originalSize} bytes\nCompressed size: ${compressedSize} bytes\n${compressionWasEffective ? 'The compression was effective :)' : 'The compression was not effective :('}`,
    textColor: compressionWasEffective ? 'bg-green-500' : 'bg-red-500',
  };
}

function showNotificationCard(message, textColor, cardElement) {
  const timeVisible = 6 * SECOND;

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
