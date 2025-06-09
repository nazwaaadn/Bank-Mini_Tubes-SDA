Petunjuk penggunaan aplikasi:

Run User
1. ./buildUser.bat
2. ./mainUser

Run Admin
1. ./build.bat
2. ./mainAdmin

Menu User
1. Register atau Login Terlebih Dahulu
Jika belum punya akun, user harus mendaftar terlebih dahulu melalui menu Register. Jika sudah punya akun, login dengan input nama dan password.

2. Pesan tiket
2.1. Pilih Menu "Pesan Tiket"
2.2. Sistem akan menampilkan daftar bus yang masih dalam antrian (siap berangkat).
2.3. User diminta memasukkan idBus dari bus yang ingin dipilih.
2.4. Input Terminal Keberangkatan dan Tujuan
2.5. Tiket Terbuat 

3. Melihat Tiket yang Sudah Dipesan
3.1. Kembali ke menu awal (setelah login), lalu pilih menu “Lihat Tiket Aktif”.
3.2. Sistem akan menampilkan semua tiket “aktif” yang masih berlaku (bus belum berangkat).

4. Melihat Semua Tiket (Aktif, Done, Cancelled)
4.1. Kembali ke menu awal, lalu pilih menu “Lihat Semua Tiket”.
4.2. Sistem akan menampilkan seluruh riwayat tiket user, baik yang aktif, sudah selesai, atau dibatalkan.

5. Melakukan perjalanan
5.1. Kembali ke menu awal, pilih Lakukan perjalanan

6. Keluar (exit) jika sudah selesai

Menu Admin
Login terlebih dahulu, dengan:
IDAdmin = admin
password = adminkece

1. Pilih menu Manajemen Bus
1.1. Input data Bus Baru
Admin akan diminta untuk mengisi beberapa informasi berikut:
idBus (ID unik bus)
Nama Supir
Plat Nomor
Kapasitas (jumlah maksimal penumpang)
Terminal Awal (pemberangkatan)
Terminal Tujuan (tujuan akhir)
Waktu Berangkat (format: hh:mm)
Waktu Tiba (format: hh:mm)

1.2. Hapus bus
1.2.1 Pilih menu hapus bus
1.2.2. Input idBus
1.2.3. DataBus terhapus

1.3. Lihat Daftar Bus
1.3.1. Pilih menu Lihat daftar Bus
1.3.2. Data semua bus pada file dataBus.txt tampil

2. Pilih menu masukkan bus ke antrian
2.1. input idBus
2.2. Bus masuk ke antrian

3. Pilih menu release bus dari antrian
3.1. input idBus
3.2. Bus keluar dari antrian, siap melakukan perjalanan

4. Lihat histori transaksi tiket
4.1. Pilih menu histori transaksi

5. Lihat terminal
5.1. Pilih menu lihat terminal

6. Keluar (exit) jika sudah selesai