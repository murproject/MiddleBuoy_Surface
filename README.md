 # Прошивка для платы пульта MiddleBuoy

Подробнее о наборе [MiddleBuoy](https://wiki.murproject.com/ru/MiddleBuoy/buoy-instruction).

Данная прошивка предназначена для платы пульта управления MiddleBuoy. Пульт представляет собой плату с кнопками и дисплеем.

> Если нужна прошивка для платы буя, то вам нужен репозиторий [MiddleBuoy_Bottom](https://github.com/murproject/MiddleBuoy_Bottom).

Для прошивки, подключите USB-кабель к плате и воспользуйтесь [Arduino IDE](https://www.arduino.cc/en/Main/Software). Выберите нужный последовательный порт и плату Arduino Leonardo. Обратите внимание на необходимые библиотеки.

## Установка библиотек

Для прошивки потребуются библиотеки: 
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [GyverOled](https://github.com/GyverLibs/GyverOLED)

Чтобы установить эти библиотеки откройте менеджер библиотек в Arduino IDE: Скетч / Покдлючить библиотеку / Управлять библиотеками. Введите в поиск **Adafruit NeoPixel** и установите библиотеку. Аналогичным образом установите **GyverOled**.