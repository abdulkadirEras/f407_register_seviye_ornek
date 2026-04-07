Bu proje, STM32F407VGT6 Discovery geliştirme kartı üzerinde donanım birimlerinin doğrudan Register (Kaydedici) seviyesinde nasıl kontrol edildiğini gösteren örnek uygulamaları içerir. Yüksek seviyeli kütüphaneler (HAL/LL) yerine doğrudan işlemci mimarisine odaklanılarak yazılmıştır.

🚀 İçerik ve Özellikler
Proje içerisinde aşağıdaki donanım birimleri ve özellikler Register seviyesinde yapılandırılmıştır:

Timer Interrupt: Zamanlayıcı kesmeleri kullanılarak hassas zaman yönetimi.

PWM (Pulse Width Modulation): Sinyal genişlik modülasyonu ile donanım kontrolü.

Çoklu ADC Okuma (DMA ile): Analog verilerin işlemciyi yormadan DMA (Direct Memory Access) üzerinden yüksek hızla okunması.

IWDG (Independent Watchdog): Sistemin kilitlenmesi durumunda otomatik olarak yeniden başlatılmasını sağlayan bekçi köpeği uygulaması.

🛠 Donanım ve Yazılım Gereksinimleri
Geliştirme Kartı: STM32F407VGT6 Discovery (Cortex-M4)

IDE: STM32CubeIDE veya Keil uVision

Derleyici: GNU Arm Embedded Toolchain

📝 Notlar
Bu çalışma, gömülü sistemlerde donanım mimarisini daha derinlemesine anlamak ve çevre birimlerini düşük seviyede optimize etmek isteyenler için eğitim amaçlı hazırlanmıştır.

---------------------------------------------------------------------------
This repository contains sample applications developed for the STM32F407VGT6 Discovery board, focusing on direct Register-level hardware control. The project aims to demonstrate hardware peripheral configuration without relying on high-level libraries (such as HAL or LL), providing a deeper understanding of the processor architecture.

🚀 Features & Implementation
The project includes register-level configurations for the following peripherals:

Timer Interrupt: Precise time management using hardware timers and interrupts.

PWM (Pulse Width Modulation): Manual signal modulation for controlling hardware components.

Multi-Channel ADC with DMA: High-speed analog-to-digital conversion using Direct Memory Access (DMA) to minimize CPU load.

IWDG (Independent Watchdog): Implementation of the watchdog timer to ensure system reliability and automatic resets in case of failure.

🛠 Hardware & Software Requirements
Development Board: STM32F407VGT6 Discovery (Cortex-M4)

IDE: STM32CubeIDE, Keil uVision, or any ARM-compatible toolchain.

Compiler: GNU Arm Embedded Toolchain.

📝 Purpose
This project is designed for educational purposes to explore embedded systems at a low level, optimize peripheral performance, and master the STM32 hardware manual.
