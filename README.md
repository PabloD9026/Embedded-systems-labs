# Embedded-systems-labs

Лабораторные работы по дисциплине встраиваемые системы

Отчёты доступны по ссылкам:
  
  1.https://drive.google.com/file/d/1BM94UvZRSnQPHgVjrORH7YOFdb20vaOR/view?usp=sharing
  
  2.

# Лабораторная работа №1 - Знакомство с SPI

# Задача

Получение передачи данных с помощью интерфейса передачи данных SPI между 4-ями устройствами на базе STM32. 

# SPI - Serial Peripheral Interface

Название интерфейс SPI является аббревиатурой от “Serial Peripheral Bus”, что можно перевести как “шина для подключения периферийных устройств”. Отсюда вытекает ее главное назначение - связать одно главное устройство - Ведущее (Master) - с одним или несколькими Ведомыми (Slave). Ведущий в этом интерфейсе всегда один, только он руководит всем процессом, и только он может формировать тактовые импульсы. Если в нашем случае Ведущим всегда является микроконтроллер (эту роль может исполнять и компьютер, но это другая история), то Ведомыми в подавляющем большинстве случаев являются именно периферические устройства. Датчики, дисплеи, микросхемы ЦАП и АЦП, RFID-ридеры, модули беспроводной связи, включая приемо-передатчики WiFi и Bluetooth, GPRS-адаптеры и так далее. Особенно этот интерфейс востребован там, где требуется высокая скорость передачи данных и не менее высокая надежность. SPI именно такой, он самый быстрый из всех имеющихся в нашем распоряжении и самый “легкий” с точки зрения потребляемых ресурсов. Расплатой за это является использование большего количеств проводов, чем для других интерфейсов. Здесь требуется 3 штуки только непосредственно для передачи данных:

  •	MOSI - Master Output Slave Input (Ведущий передает, Ведомый принимает).
  
  •	MISO - Master Input Slave Output (Ведущий принимает, Ведомый передает).
  
  •	SCLK, иначе SCK - Serial Clock (тактовый сигнал).

Есть три способа подключения Ведущего и Ведомых.

  •	Первый: если Ведомый один, одноименные пины просто соединяются напрямую. 
  
  •	Второй: классический. Несколько Ведомых соединяются с Ведущим параллельно шинами MISO, MOSI и SCK, а SS ведет к каждому из них индивидуально.
  
  •	Третий: цепочка, он же кольцо. SS один на всех, но данные передаются как бы сквозь устройства с одного на другое. Пока SS в низком уровне, данные растекаются по своим местам, после подъема уровня SS, устройства начинают с принятыми данными работать. 

Как было выше сказано, интерфейс передачи данных SPI – синхронный. Это означает, что он использует отдельные линии для данных и «тактирование», которое обеспечивает идеальную синхронизацию обеих сторон. Тактовый сигнал представляет собой колебательный сигнал, который точно сообщает приемнику, когда следует производить выборку битов в линии данных. Это может быть нарастающий (от низкого к высокому) или падающий (от высокого к низкому) фронт тактового сигнала. Когда приемник обнаруживает этот фронт, он немедленно смотрит на строку данных, чтобы прочитать следующий бит. Поскольку тактирование передаётся вместе с данными, указание скорости не имеет значения, хотя устройства будут иметь максимальную скорость, с которой они могут работать. Схема показана ниже:

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/f2b708e3-bbe3-42c6-bd11-0e4dcffafd7a)

# Выполнение работы

В рамках этой лабораторной работы был использован классический способ подключения SPI, где для каждого ведомого предназначен и подключен к нему SS пин. 

В качестве мастера был произвольно выбран микроконтроллер STM32103C8T6 или bluepill.его распиновка показана ниже:

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/f85a9252-0255-4327-9ce8-64cd4fbb5568)

и его настройки:

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/6e421f07-35bd-4a5d-ba4e-ce556142fc7c)

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/bab42787-5658-49a9-a926-b4ee2c0ddb6e)

Пины SS – PA1, PA2 и PA4 для slave1, slave2 и slave3 соответственно. Так же на рисунке можно посмотреть, что прерывание было подключено. Это было сделано для того, чтобы не нагружать микроконтроллер, а чтобы он только занимался передачей данных, когда надо.

Потом для всех SS пинов, была настроена максимальная скорость переключения, и чтобы начальное состояние пинов было HIGH или логический 1.

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/98231520-67c6-45a1-a66d-57065f5aaaec)

В качестве Slave были использованы платы STM32 Nucleo 144 на базе STM32F429ZIT6, так как все имеют одинаковые настройки только будет показан один пример настроек.

Распиновка

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/0cb43a50-aba0-4feb-bd7a-2124249eb599)

Где пин PB14 это светодиод, которым управляем с мастера через SPI.

Настройки

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/0f60ef12-05f0-4051-ad8b-026fe9328de8)

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/49c11c0d-f1ff-400d-a753-2da7596d04de)

В настройках Slave микроконтроллеров была включена функция Hardware NSS Signal для того, чтобы микроконтроллер начал автоматический принять данные при сигнале мастера.

Схема подключения электроники сделана по схеме показана ниже

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/f431f20a-6975-4ff7-8f88-1f75d365fc90)

С помощью SPI были отправлены массивы с Мастера и Slave. В массиве было указано каким, из двух светодиодов встроенных на плате, управлять. 

Результат работы

# Лабораторная работа №2 - Знакомство с USB

В ходе данной работы необходимо связать микроконтроллеры на бызе STM32 посредством USB подключения.

Необходимо отрисовывать апериодическое звено 2-го порядка на светодиодной панели P10 посредством передачи сигналов по SPI с отладочной платы STM32 BluePill, которая в свою очередь получает значения функции по USB соединению из другой платы STM32 Bluepill. Должна быть возможность вносить в систему возмущение посредством нажатия кнопки.

Для выполнения задания необходимо изучить принцип работы USB соединения и P10 светодиодную панель.

# USB - Universal Serial Bus



# P10 Светодиодная панель

P10 Светодиодная панель или P10 Led Board представляет собой панель состоящую из корпуса, светодиодов и управляющих микросхем. Размерность светодиодной панели в пикселях равна 16 по высоте и 32 по ширине. Управление светодиодной панелью осуществляется через интерфейс SPI, что позволяет подключать несколько панелей последовательно и масштабиовать размерность панели.

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/230f2659-26ea-4239-9b96-57cc598d365f)

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/611f7046-9bf6-4dc3-86a0-c60d3860895a)

Распиновка входного разъёма

![image](https://github.com/PabloD9026/Embedded-systems-labs/assets/91127156/b3065a55-d843-4ee5-a29f-c43101d15114)

На разъёме:

  Пины 6, 14, 16 (C, G, D) — не используются (возможно они используются в 3-цветных матрицах).
  
  Пины 2 и 4 (A и B) — задают, какая из 4 групп светодиодов экрана работает в данный момент. Матрицы используют динамическую индикацию, поочерёдно переключая 4 группы светодиодов в зависимости от логических уровней на ножках A и B. На плате эти сигналы приходят на дешифратор D18, который открывает 1 из 4 групп P-канальных полевиков, тем самым подавая +5В на аноды светодиодов выбранной группы.
  
  Пин 1 (nOE) — разрешает работу матрицы (лог. 0 гасит все матрицы в цепочке). Логика работы этого пина реализована на элементах НЕ в D19 и дешифраторе D18.
  
  Пины 8 и 12 (CLK и R) — линии клока и данных синхронного последовательного интерфейса. Их подключаем к SCK и MOSI интерфейса SPI микроконтроллера.
  
  Пин 10 (SCLK) — по переднему фронту защёлкивает переданные в сдвиговые регистры данные на их выходы. Сдвиговые регистры подключены к катодам светодиодов матрицы. По этой причине передаваемые данные нужно инвертировать (светодиод будет гореть при лог. 0)
  















