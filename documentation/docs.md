# ~~Документация~~ Примечания
- Main Service на первый взгляд кажется сомнительным решением – единая точка отказа для всего сервиса, заметный боттлнек. Но у такого полхода есть и положительная сторона: один инстанс этого сервиса является автономной независимой единицей, в которой происходит обработка основной логики сервиса (в то время как User Service и Route Service – это легковесные обёртки над базами данных). За счёт этого этот сервис можно легко масштабировать горизонтально, просто добавив инстансы и поместив их за балансировщик. И это масштабирование улучшает работоспосоность не одного лишь компонента Main Service, а всего сервиса поиска попутчиков целиком!
- Также могут возникнуть вопросы к трём раздельным PostgreSQL базам данных. Такое решение было принято, чтобы для разных баз можно было применить разный набор настроек. Например, база данных пользователей скорее всего будет сильно больше читаться, чем изменяться, поэтому её можно попробовать соптимизировать для чтения. В то же время база маршрутов будет часто редактироваться, поэтому её лучше оптимизировать на запись. Также такой подход полезен и для масштабирования – можно добавлять ресурсы в конкретные базы, что может быть полезно, так как скорее всего количество поездок будет больше, чем количество маршрутов и пользователей, вместе взятые, поэтому ресурсы базе с поездками понадобятся.
- В HTTP-запросах, где используется user_id/route_id/trip_id, предполагается, что они были получены в предыдущих запросах. Можно также реализовать дополнительные варианты ручек, на случай, когда id-шники неизвестны (в таком случае за ними сначала придётся сходить в соответствующую базу, и тут вскрывается ещё один плюс подхода с Main Service, заключающийся в том, что все эти походы может сделать Main Service, а пользователю не надо больше никуда отправлять запросы). Пример такого дополнительного варианта я привёл для сценария с получением маршрутов пользователя, остальное делается аналогично.