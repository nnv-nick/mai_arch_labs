Запустить всё можно с помощью команды `bash start_lab_02.sh`, запущенной из корневой директории.
После этого нужно подождать какое-то время, пока образы поднимутся (когда в логах появятся записи вида `lab_02_trips    | # Connecting to mongodb: mongodb_lab2:27017`, значит, всё готово, и сервис может принимать запросы).

После этого чтобы заполнить базу данных тестовыми значениями, можно запустить питоновский скрипт `fill_db_with_data.py`, находящийся в этой же папке.

Моя реализация поддерживает функциональность, описанную в файле с заданиями в моём варианте:
- Создание маршрута
- Получение маршрутов пользователя
- Создание поездки
- Подключение пользователей к поездке
- Получение информации о поездке

В моём понимании задания маршрут – это некоторый набор остановок с дополнительной информацией (время прохождения, цена и т.п.). У маршрута также есть автор (поле `user_id`). Поездка – это сущность, составленная на основе маршрута и имеющая в своём составе `id` предварительно созданного маршрута. В поездке по маршруту могут участвовать пользователи, которые ищут в эту поездку попутчиков. Соответственно, в составе поездки (`trip`) есть также поле `user_ids` – `id` пользователей, которые в ней участвуют.

Подробнее о составе структур поездок и маршрутов, а также о способах взаимодействия с ними и с сервисом можно узнать из спецификации Open API (файл `trips.yaml` в этой же папке).