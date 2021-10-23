## Đồ án kỹ thuật vi điều khiển - CT581

##### Mode đo: có hai mode 0 và 1:
* Mode 0: Đo áp suất, đơn vị Kpa
* Mode 1: Đo mực nước, đơn vị cm
##### Phân biệt các mode đo:
Phân biệt 2 mode thông qua dấu '.' ở hàng đơn vị của màn hình led, có dấu '.' là mode đo mực nước, ko có là đo áp suất bình thường.
##### Sơ lược hoạt động:
* Khởi động -> màn hình hiện ra 'load'
* Chờ 1-2p (thực tế khoảng vài giây, trong proteus bị chậm ko rõ lý do)
* Sau khi khởi động xong, mặc định vào mode 
##### Lưu ý
...