import json
import subprocess

jwt_token_header = '-H "Authorization: Basic dGpvaW9udzplWFZ3ZVhkMw=="'

sample_route = '{"duration":10000,"id":4,"length":150,"price":100000,"stops":[{"address":"Chelyabinsk, st. Pushkina, 12","country":"Russia","time_on_stop":200},{"address":"Moscow, st. akademika Komarova, 5a","country":"Russia","time_on_stop":1000},{"address":"Berlin, st. Guteldorf, 14","country":"Germany","time_on_stop":350}],"title":"Pohod 1","user_id":3}'
route_json = json.loads(sample_route)
for cur_id in range(5):
    for cur_user_id in range(5):
        route_json["id"] = cur_id + cur_user_id * 5
        route_json["user_id"] = cur_user_id
        subprocess.run(f'curl -X POST {jwt_token_header} -H "Content-Type: application/json" -d \'{json.dumps(route_json)}\' http://localhost:8888/route', shell=True)

trip_json = {"id": 1, "route_id": 3, "user_ids": [0, 3]}
subprocess.run(f'curl -X POST {jwt_token_header} -H "Content-Type: application/json" -d \'{json.dumps(trip_json)}\' http://localhost:8888/trip', shell=True)
trip_json = {"id": 2, "route_id": 14, "user_ids": [1, 2, 4]}
subprocess.run(f'curl -X POST {jwt_token_header} -H "Content-Type: application/json" -d \'{json.dumps(trip_json)}\' http://localhost:8888/trip', shell=True)
trip_json = {"id": 3, "route_id": 13, "user_ids": []}
subprocess.run(f'curl -X POST {jwt_token_header} -H "Content-Type: application/json" -d \'{json.dumps(trip_json)}\' http://localhost:8888/trip', shell=True)
trip_json = {"id": 4, "route_id": 21, "user_ids": [0, 1, 2, 3, 4]}
subprocess.run(f'curl -X POST {jwt_token_header} -H "Content-Type: application/json" -d \'{json.dumps(trip_json)}\' http://localhost:8888/trip', shell=True)
