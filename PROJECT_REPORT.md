# PROJE RAPORU: My Sweet Home (MSH)
**Ders:** CENG-464 Design Patterns with C++  
**Tarih:** 17 Aralık 2025  
**Takım:** Internal Pointer Variable

---

## 1. Yönetici Özeti
My Sweet Home (MSH), Nesnelerin İnterneti (IoT) cihazlarını simüle eden ve yöneten, C++ tabanlı, ölçeklenebilir bir akıllı ev otomasyon sistemidir. Bu proje, yazılım mühendisliğinde karşılaşılan yaygın sorunlara **Tasarım Kalıpları (Design Patterns)** kullanılarak nasıl zarif çözümler getirilebileceğini göstermek amacıyla geliştirilmiştir.

Sistem, son versiyon (v4 + Web) itibarıyla hem komut satırı (CLI) hem de modern bir Web Arayüzü üzerinden kontrol edilebilmektedir. Dockerizasyon sayesinde platform bağımsız çalışabilir hale getirilmiştir.

---

## 2. Yazılım Mimarisi ve Tasarım Kalıpları Analizi

Projenin temel taşı, "Spagetti Kod"dan kaçınmak ve SOLID prensiplerine sadık kalmaktır. Aşağıda projede kullanılan kalıplar ve kullanım gerekçeleri detaylandırılmıştır.

### 2.1. Yaratımsal Kalıplar (Creational Patterns)

#### **Singleton (Tekil Nesne)**
*   **Kullanım Yeri:** `Alarm` ve `Storage` sınıfları.
*   **Problem:** Sistemde birden fazla alarm sisteminin çalışması senkronizasyon hatasına yol açar. Loglama işleminin (`Storage`) tek bir dosyaya sırayla yazılması gerekir.
*   **Çözüm:** Bu sınıfların constructor'ları `private` yapılmış ve `getInstance()` metodu ile tek bir örneğe erişim sağlanmıştır.

#### **Factory Method**
*   **Kullanım Yeri:** `SimpleDeviceFactory`.
*   **Problem:** Kullanıcı arayüzünden gelen girdiye ('L' for Light, 'T' for TV) göre nesne oluştururken `if-else` bloklarını ana koddan soyutlamak.
*   **Çözüm:** Nesne yaratım süreci bir fabrikaya devredilmiştir.

#### **Abstract Factory**
*   **Kullanım Yeri:** `DeviceFactory`, `DetectorFactory` (NestFactory vs BudgetFactory).
*   **Problem:** Cihazların markalarına veya kalitelerine göre (Örn: Lüks set vs Ekonomik set) gruplar halinde üretilmesi.
*   **Çözüm:** Birbiriyle ilişkili ürün ailelerinin (Sensor + Alarm) uyumlu bir şekilde üretilmesi sağlanmıştır.

#### **Prototype**
*   **Kullanım Yeri:** `Device::clone()`.
*   **Problem:** Bir cihazın ayarlarını (parlaklık, ses seviyesi) kopyalayarak yeni bir cihaz eklemek istendiğinde, sıfırdan oluşturmak yerine var olanı kopyalamak.
*   **Çözüm:** Mevcut nesnenin kopyasını döndüren bir `clone` metodu implemente edilmiştir.

### 2.2. Yapısal Kalıplar (Structural Patterns)

#### **Facade (Ön Yüz)**
*   **Kullanım Yeri:** `HomeController`.
*   **Problem:** `Light`, `Camera`, `SecuritySystem`, `ModeManager` gibi onlarca alt sistemin karmaşıklığını `main.cpp` veya Web Sunucusu'ndan gizlemek.
*   **Çözüm:** `HomeController` sınıfı, tüm sistemi başlatan, yöneten ve kapatan basit bir arayüz sunar. Web arayüzü sadece bu sınıfla muhatap olur.

### 2.3. Davranışsal Kalıplar (Behavioral Patterns)

#### **State (Durum)**
*   **Kullanım Yeri:** `ModeManager` (Normal, Party, Cinema) ve `SystemState`.
*   **Problem:** "Party" modundayken ışıkların davranışı ile "Cinema" modundaki davranışlarının farklı olması. `if(mode == Party)` bloklarından kaçınmak.
*   **Çözüm:** Her mod bir sınıf olarak tasarlanmış, sistemin o anki durumuna göre davranış değiştirmesi sağlanmıştır.

#### **Memento (Hatıra)**
*   **Kullanım Yeri:** `StateManager`, `HomeMemento`.
*   **Problem:** Kullanıcı hatalı bir işlem yaptığında "Geri Al" (Undo) veya "Önceki Duruma Dön" özelliğinin gerekmesi.
*   **Çözüm:** Sistemin anlık görüntüsü (`HomeMemento`) saklanır ve istendiğinde geri yüklenir.

#### **Observer (Gözlemci)**
*   **Kullanım Yeri:** `NotificationSystem`, `IDeviceObserver`.
*   **Problem:** Bir cihaz bozulduğunda veya alarm çaldığında, ilgililerin (Log sistemi, Kullanıcı ekranı) haberdar edilmesi.
*   **Çözüm:** Cihazlar (Subject) durum değiştiğinde abonelerine (Observer) haber verir.

#### **Chain of Responsibility (Sorumluluk Zinciri)**
*   **Kullanım Yeri:** `SecuritySystem` (Hareket Algılama Senaryosu).
*   **Problem:** Hareket algılandığında sırasıyla: 1. Alarm Çal -> 2. Işıkları Yak -> 3. Polisi Ara.
*   **Çözüm:** Her işlem bir sonrakini tetikler. İşlem zinciri dinamik olarak kurgulanabilir.

#### **Template Method**
*   **Kullanım Yeri:** `Device::powerOn()`.
*   **Problem:** Her cihaz açılırken log tutulmalı ve statüsü güncellenmelidir, ancak açılma "efekti" cihazdan cihaza değişir.
*   **Çözüm:** Ana algoritma iskeleti `Device` sınıfında tanımlanmış, spesifik adımlar alt sınıflara bırakılmıştır.

---

## 3. Teknoloji Yığını ve Yenilikler (Web Entegrasyonu)

Proje, klasik C++ mimarisinin modern teknolojilerle nasıl konuşabileceğini kanıtlamaktadır.

*   **Backend (Core):** C++98 (Performans ve Donanım Kontrolü için).
*   **Middleware:** Node.js & Socket.io (C++ sürecini yönetmek ve Web soketi açmak için).
*   **Frontend:** HTML5, CSS3 (Tailwind), JavaScript (Canlı Terminal emülasyonu).
*   **Deployment:** Docker (Multi-stage build ile optimize edilmiş konteyner).

### Web Arayüzü Özellikleri
1.  **İzole Oturumlar:** Her kullanıcı için sunucuda geçici ve izole bir çalışma alanı yaratılır.
2.  **Güvenlik:** Girdiler Regex ile filtrelenir, oturumlar 5 dakika sonunda otomatik imha edilir.
3.  **Görsellik:** Matrix tarzı canlı terminal, Dark/Light mod desteği.

---

## 4. Sonuç
My Sweet Home projesi, sadece bir ev otomasyonu değil, aynı zamanda sağlam bir yazılım mimarisi örneğidir. Design Pattern'lerin doğru kullanımı sayesinde sistem; bakımı kolay, genişletilebilir ve modern arayüzlere (Web) kolayca entegre edilebilir hale gelmiştir.
