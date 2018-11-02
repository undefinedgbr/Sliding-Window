# sliding_window
Sliding Window Protocol Simulation with Selective Repeat Algorithm

## Petunjuk Penggunaan Program

### Server / Receiver
```
./server.out <filename> <windowsize> <buffersize> <port>
```
Keterangan :
1. filename diisi nama file yang diinginkan untuk diisi dengan data yang diterima
2. windowsize diisi ukuran *window* yang diinginkan
3. buffersize diisi ukuran *buffer* yang diinginkan
4. port diisi dengan *port* yang digunakan untuk membangun koneksi dengan penerima

### Client / Sender
```
./client.out <filename> <windowsize> <buffersize> <destination_ip> <port>
```

Keterangan:
1. filename diisi nama file yang ingin dikirimkan
2. windowsize diisi ukuran *window* yang diinginkan
3. buffersize diisi ukuran *buffer* yang diinginkan
4. destination_ip diisi dengan *ip address* dari server tujuan
5. port diisi dengan *port* yang digunakan untuk membangun koneksi dengan server tujuan

## Cara Kerja Sliding Window
*Sliding Window Protocol* adalah protokol *control flow* untuk pengiriman data berbasis paket. Sliding Window memecah file yang ingin dikirim menjadi beberapa paket (dengan ukuran tertentu) dan memberi nomor (*sequence number*) pada setiap paket untuk mengindentifikasi paket yang hilang.

Pada protokol ini, ada yang disebut dengan *window*. *window* pada *client* digunakan untuk mengetahui paket mana yang sudah atau akan dikirim, namun belum menerima *acknowledgement* (disebut ACK)bahwa  paket sudah diterima server. *Client* akan mengirimkan beberapa paket sekaligus sejumlah *window size* dan *window* akan "bergeser" ketika sudah menerima ACK. Paket yang sudah dikirim dapat dikirim ulang oleh *client* jika *client* sudah menunggu melewati batas waktu (*timeout*) atau menerima NAK (ACK yang memberi tahu bahwa paket yang diterima server error).

Ketika paket diterima, server akan mendeteksi apakah paket tersebut error atau tidak. Jika paket yang diterima tidak terdeteksi error, maka server akan mengirimkan ACK pada client dan "menggeser" window pada server. Jika terdeteksi error, server akan mengirimkan NAK ke client. Server dapat mengetahui apakah pengiriman paket sudah selesia jika menerima paket dengan SOH tertentu.

## Kelas-kelas pada program

### file_reader

File ini berisi kelas FileReader yang berfungsi untuk membaca file dan memecah file tersebut menjadi beberapa frame (paket).

### file_writer

File ini berisi kelas FileWriter yang berfungsi untuk menuliskan paket-paket yang sudah diterima kedalam file tujuan.

### packet

Kelas paket adalah *parent* dari kelas ACK dan Frame yang akan ditransfer oleh *client* dan *server*. Kelas ini memiliki atribut seqNum (nomor paket) dan checksum (untuk pendeteksi error pada pengiriman).

Kelas ini memiliki fungsi checkValidity yang akan mendeteksi error berdasarkan checksum dari packet yang diterima dengan checksum yang di-*generate* dari data yang diterima. Selain itu kelas ini memiliki fungsi *pure virtual* generate checksum dan serialize untuk mengubah objek packet menjadi array of char.

### ACK

ACK adalah subkelas dari packet yang memiliki atribut ack (sebagai penentu ACK atau NAK), nextSeqNum, dan checksum (untuk pendeteksi error). Kelas ini adalah representasi dari ACK yang akan dikirimkan oleh server sebagai *acknowledgement* diterimanya paket.


### frame

Frame adalah kelas yang memiliki atribut SOH (sebagai penanda paket terakhir), seqNum (penanda nomor paket), dataLength, data, dan atribut statik frameCount (jumlah frame).
Kelas ini adalah representasi packet/frame yang dikirimkan oleh client yang berisikan data file yang sudah dipecah.

### sliding_window

Kelas ini adalah representasi sliding window yang akan dimiliki oleh server dan client. Kelas ini memiliki atribut size, start, end, availableFrame, dumped, locked, frames (Vector of frames).

### sliding_window 

### Sisi client

1. Client akan melakukan koneksi ke socket.
2. File yang dipilih oleh client akan dibaca dan dipecah menjadi frame oleh reader.
3. Frame tersebut akan di-*generate* checksum-nya dan dikirimkan oleh client dengan fungsi **sendMessage**.
  1. pada sendMessage, frame yang akan dikirim, akan di-*serialize* terlebih dahulu dan dimasukan ke sebuah variable buffer.
  2. Frame tersebut akan dimasukan ke window untuk dicek apakah frame tersebut bisa dikirim atau tidak (**this->window.addFrame()**).
  3. Jika bisa, client akan mengirimkan buffer ke server dan mulai menghitung waktu untuk timeout dengan memasukannya ke deque timeouts.
4. Client akan menjalankan dua thread. Thread yang pertama adalah menunggu ACK/NAK dari server (**listenForResponse()**) dan thread yang kedua adalah untuk mengirimkan ulang packet jika ada frame yang sudah timeouts (**resendIfTimeout()**).
5. Ketika menerima NAK, client akan mengirimkan ulang paket yang error tersebut. Semua pengirimkan akan dilakukan dengan fungsi resendFrame().
6. Ketika menerima ACK, window client akan "bergeser" dengan fungsi **this->window.addACK()**


### Sisi Server
1. Server akan melakukan koneksi ke socket.
2. Server akan me-*listen* pada port yang ditentukan untuk menerima frame secara looping. (**listenForClients()**)
3. Frame yang diterima (dalam bentuk array of char) akan dimasukan ke variabel buffer dan dikonversi menjadi tipe frame. Frame tersebut lalu akan diproses oleh fungsi **processFrame()**
  1. Frame yang diproses akan dicek kebenarannya (**checkValidity()**) untuk mendeteksi error.
  2. Jika terdeteksi error, server akan mengirimkan NAK kepada client (**replyNACK()**)
  3. Jika tidak terdeteksi error, server akan mengirimkan ACK kepada client (**replyACK()**) dan "menggeser" window pada server (**this->window.addACK(ack)**).
4. Setelah frame diproses, frame apakah frame tersebut merupakat frame terakhir atau tidak dari SOH-nya.
5. Jika frame tersebut adalah frame terakhir, akan diperksa kelengkapan semua frame dengan **checkAllFrames()**.
6. Jika frame sudah lengkap, server akan terminate.
7. Jika tidak lengkap, server akan terus menunggu frame hingga sudah lengkap.
