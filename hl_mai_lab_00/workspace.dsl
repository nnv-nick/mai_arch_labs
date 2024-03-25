workspace {
    name "Сервис поиска попутчиков"
    description "Сервис поможет Вам найти хорошиъ попутчиков в любую поездку!"

    !identifiers hierarchical

    !docs documentation

    model {
        properties { 
            structurizr.groupSeparator "/"
        }

        user = person "Пользователь сервиса поиска попутчиков"
        companion_search = softwareSystem "Сервис поиска попутчиков" {
            description "Сервис для поиска попутчиков, составления маршрутов и многого другого!"

            main_service = container "Main service" {
                description "Сервис обработки всех пользовательских запросов"
            }

            user_service = container "User service" {
                description "Сервис управления пользователями"
            }

            route_service = container "Route service" {
                description "Сервис управления маршрутами"
            }

            group "Слой данных" {
                user_database = container "User Database" {
                    description "База данных для хранения пользователей"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                trip_database = container "Trip Database" {
                    description "База данных для хранения поездок"
                    technology "PostgreSQL 15"
                    tags "database"
                }

                route_database = container "Route Database" {
                    description "База данных для хранения маршрутов"
                    technology "PostgreSQL 15"
                    tags "database"
                }
            }

            user_service -> user_database "Получение/обновление данных о пользователях" "TCP 5432"

            route_service -> route_database "Получение/обновление данных о маршрутах" "TCP 1832"

            main_service -> user_service "Операции с данными о пользователях" "REST HTTP:8080"
            main_service -> route_service "Операции с данными о маршрутах" "REST HTTP:8080"
            main_service -> trip_database "Получение/обновление данных о поездках" "TCP 9011"

            user -> main_service "Все операции с сервисом" "REST HTTP:8080"
        }

        user -> companion_search "Получение/обновление данных о поездках/маршрутах/пользователях"

        deploymentEnvironment "Production" {
            deploymentNode "User Server" {
                containerInstance companion_search.user_service
            }

            deploymentNode "Route Server" {
                containerInstance companion_search.route_service
            }

            deploymentNode "Main Server" {
                containerInstance companion_search.main_service
                instances 5
            }

            deploymentNode "databases" {
     
                deploymentNode "Database Server 1" {
                    containerInstance companion_search.user_database
                }

                deploymentNode "Database Server 2" {
                    containerInstance companion_search.trip_database
                    instances 3
                }

                deploymentNode "Database Server 3" {
                    containerInstance companion_search.route_database
                }
            }
        }
    }

    views {
        themes default

        properties { 
            structurizr.tooltips true
        }

        !script groovy {
            workspace.views.createDefaultViews()
            workspace.views.views.findAll { it instanceof com.structurizr.view.ModelView }.each { it.enableAutomaticLayout() }
        }

        dynamic companion_search "UC01" "Добавление нового пользователя" {
            autoLayout
            user -> companion_search.main_service "Создать нового пользователя (POST /user)"
            companion_search.main_service -> companion_search.user_service "Создать нового пользователя (POST /user)"
            companion_search.user_service -> companion_search.user_database "Сохранить данные о пользователе" 
        }

        dynamic companion_search "UC02" "Поиск пользователя по логину" {
            autoLayout
            user -> companion_search.main_service "Найти пользователя по логину (GET /user?login=...)"
            companion_search.main_service -> companion_search.user_service "Найти пользователя по логину (GET /user?login=...)"
            companion_search.user_service -> companion_search.user_database "Поиск пользователя по логину" 
        }

        dynamic companion_search "UC03" "Поиск пользователя по маске имя и фамилии" {
            autoLayout
            user -> companion_search.main_service "Найти пользователя по маске имя и фамилии (GET /user?name=...&surname=...)"
            companion_search.main_service -> companion_search.user_service "Найти пользователя по маске имя и фамилии (GET /user?name=...&surname=...)"
            companion_search.user_service -> companion_search.user_database "Поиск пользователя по маске имя и фамилии" 
        }

        dynamic companion_search "UC04" "Создание маршрута" {
            autoLayout
            user -> companion_search.main_service "Создать маршрут (POST /route)"
            companion_search.main_service -> companion_search.user_service "Аутентификация пользователя (GET /auth)"
            companion_search.user_service -> companion_search.user_database "Аутентификация пользователя" 
            companion_search.main_service -> companion_search.route_service "Создать маршрут (POST /route)"
            companion_search.route_service -> companion_search.route_database "Создание маршрута" 
        }

        dynamic companion_search "UC05" "Получение маршрутов пользователя (если у нас нет user_id)" {
            autoLayout
            user -> companion_search.main_service "Получить маршруты пользователя (GET /user_routes?login=... или GET /user_routes?name=...&surname=...)"
            companion_search.main_service -> companion_search.user_service "Найти пользователя (GET /user?login=... или GET /user?name=...&surname=...)"
            companion_search.user_service -> companion_search.user_database "Поиск пользователя" 
            companion_search.main_service -> companion_search.route_service "Получить маршруты пользователя (GET /user_routes?user_id=...)"
            companion_search.route_service -> companion_search.route_database "Получение маршрутов пользователя" 
        }

        dynamic companion_search "UC06" "Получение маршрутов пользователя (если у нас есть user_id)" {
            autoLayout
            user -> companion_search.main_service "Получить маршруты пользователя (GET /user_routes?user_id=...)"
            companion_search.main_service -> companion_search.route_service "Получить маршруты пользователя (GET /user_routes?user_id=...)"
            companion_search.route_service -> companion_search.route_database "Получение маршрутов пользователя" 
        }

        dynamic companion_search "UC07" "Создание поездки" {
            autoLayout
            user -> companion_search.main_service "Создать поездку (POST /trip?user_id=...&route_id=...)"
            companion_search.main_service -> companion_search.user_service "Аутентификация пользователя (GET /auth)"
            companion_search.user_service -> companion_search.user_database "Аутентификация пользователя"
            companion_search.main_service -> companion_search.trip_database "Создание поездки" 
        }

        dynamic companion_search "UC08" "Подключение пользователей к поездке" {
            autoLayout
            user -> companion_search.main_service "Подключить пользователей к поездке (POST /add_to_trip?trip_id=...)"
            companion_search.main_service -> companion_search.user_service "Аутентификация пользователя (GET /auth)"
            companion_search.user_service -> companion_search.user_database "Аутентификация пользователя"
            companion_search.main_service -> companion_search.trip_database "Подключение пользователей к поездке" 
        }

        dynamic companion_search "UC09" "Получение информации о поездке" {
            autoLayout
            user -> companion_search.main_service "Получить информацию о поездке (GET /trip_info?trip_id=...)"
            companion_search.main_service -> companion_search.trip_database "Получение информации о поездке" 
        }

        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}