# FP_SISOP19_B03

## SOAL

Buatlah sebuah music player dengan bahasa C yang memiliki fitur play nama_lagu, pause, next, prev, list lagu. Selain music player juga terdapat FUSE untuk mengumpulkan semua jenis file yang berekstensi .mp3 kedalam FUSE yang tersebar pada direktori /home/user. Ketika FUSE dijalankan, direktori hasil FUSE hanya berisi file .mp3 tanpa ada direktori lain di dalamnya. Asal file tersebut bisa tersebar dari berbagai folder dan subfolder. program mp3 mengarah ke FUSE untuk memutar musik.

Note: playlist bisa banyak

## JAWABAN

Terdapat 2 file untuk jawaban dari soal ini yaitu, fuse.c yang berguna untuk mengumpulkan semua jenis file yang berekstensi .mp3 dan juga mp3player.c yang merupakan program untuk player mp3 dimana terdapat fitur, play, pause, next, prev, dan list lagu.

Yang pertama adalah Fuse.c. Pada program ini, hal yang kami lakukan adalah membuat 2 data structure queue, dimana yang satu berguna untuk menelusuri semua folder yang ada, dan yang kedua berguna untuk menyimpan nama nama file mempunyai ekstensi .mp3. Untuk hal yang dilakukan adalah memasukkan directory /home/user ke queue pertama, lalu melakukan bfs, dengan syarat apabila yang sedang dicek merupakan sebuah folder, masukkan path folder tersebut ke dalam queue, sedangkan apabila yang sedang di cek merupakan sebuah file yang berekstensi .mp3, simpan full path dan nama file tersebut ke dalam queue yang kedua. Hal tersebut dilakukan sampai isi dari queue pertama kosong. Lalu dilanjutkan pada fungsi readdir, lakukan iterasi dari posisi awal queue kedua sampai posisi akhir queue kedua, lalu filler semua file tersebut. Dan pada fungsi read dan juga getattr, kita cari nama file yang sedang di cek pada queue kedua, apabila ditemukan, maka append full path yang telah di simpan di queue kedua ke fpath.

Source code : <a href="https://github.com/lutzid/FP_SISOP19_B03/blob/master/Fuse.c">Fuse</a>

Yang kedua adalah mp3player.c. Pada program ini, pertama-tama kita membuat menu untuk mp3playernya. Kemudian untuk menu playnya kita akan mengambil id dari input oleh pengguna kemudian kita cari nama lagu yang memiliki id tersebut dan menjalankan fungsi play menggunakan thread supaya mp3player dan playnya tetap berjalan beriringan. Untuk pause resume kami menggunakan variabel paused dimana ketika paused true maka lagu akan terpause dan ketika paused false maka lagu akan tetap diputar. Kemudian Next, kita cek terlebih dahulu apakah id lagu yang sekarang diputar itu id yang terakhir dari list apabila iya maka id akan di set 0, lalu id baru tersebut akan diincrement, cancel thread lagu sekarang dan play id yang baru. Kemudian id lagu yang kita putar sekarang akan di increment kemudian cari nama lagu dari id yang baru lalu kita cancel thread yang menjalankan lagu sekarang kemudian kita play. Untuk Prev, kita cek terlebih dahulu apakah id lagu yang sekarang diputar itu id yang pertama dari list apabila iya maka id akan di set menjadi id lagu terakhir +1, lalu id baru tersebut akan didicrement, cancel thread lagu sekarang dan play id yang baru. apabila tidak id lagu yang kita putar sekarang akan didicrement kemudian cari nama lagu dari id yang baru lalu kita cancel thread yang menjalankan lagu yang sekarang kemudian kita play. Untuk list lagu diawal ada fungsi prelist dimana di fungsi itu kita akan membaca directory folder yang di mount kemudian mengambil semua nama file didalamnya dan akan ditampilkan sesuai keinginan pengguna.

Source code : <a href="https://github.com/lutzid/FP_SISOP19_B03/blob/master/mp3player.c">Mp3player</a>
