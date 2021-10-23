## Đồ án kỹ thuật vi điều khiển - CT581

##### Mode đo: có hai mode 0 và 1:
* Mode 0: Đo áp suất, đơn vị Kpa
* Mode 1: Đo mực nước, đơn vị cm
##### Phân biệt các mode đo:
Phân biệt 2 mode thông qua dấu '.' ở hàng đơn vị của màn hình led, có dấu '.' là mode đo mực nước, ko có là đo áp suất bình thường.
##### Sơ lược hoạt động:
* Khởi động -> màn hình hiện ra 'load'
* Chờ khởi động 1-2p (thực tế khoảng vài giây, trong proteus bị chậm ko rõ lý do)
* Sau khi khởi động xong, mặc định vào mode 0
##### Lưu ý
...
##### Nguồn tham khảo
* Datasheet 'MSP430G2553' cho sơ đồ chân
    * https://www.ti.com/lit/ds/symlink/msp430g2553.pdf

* Datasheet 'MSP430x2xx Family'  cho module I2C, các thanh ghi i2c
    * https://www.ti.com/lit/ug/slau144j/slau144j.pdf

* Datasheet 'BMP180' cho khung giao tiếp đọc-ghi cảm biến, các thanh ghi giá trị cảm biến
    * https://cdn-shop.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf
