import ijson
from shapely.geometry import shape, Point

f = open('render_geojson/data//S_USA.AdministrativeForest/usa.geojson')#LNDCVR_California_State_Shape(1)/Shape/LandCover_Woodland3.geojson')
objects = ijson.items(f, 'features.item')
output = open("final.geo.json", "w+")
geometry = (o for o in objects)
print("here") 
output_features = []
#этот файл пойнтс.птс это все точки с которыми мы работаем. там 10000
points_file = open('points.pts').read()
points = points_file.split(",")
# point = Point( -121.5000000, 37.5000000)
# points = [point]
#Так как геоджсон очень блять большой, я пытался сохранить только те полигоны в которых есть эти 10к точек
#логика вышла неправильной. я не смог найти эти полигоны 
# КАРОЧ ОСТАВШАЯСЯ ПРОБЛЕМА ЭТО ТУПО НАЙТИ ТИП ТЕРРЕЙНА В ЭТИХ 10к точках
for point in points:
	print(point)
	curr = point.replace("[", "").replace("]", "").split("; ")
	temp_point = Point(-105.86660302, 37.4499334)#Point( float(curr[0]), float(curr[1]))
	is_found = 0
	for feature in output_features:
		# print(feature)
		polygon = shape(feature['geometry'])
		if polygon.contains(point):
			is_found = 1
	if is_found:
		continue
	for feature in geometry:
		# print(feature['geometry']['coordinates'][0][0])
		polygon = shape(feature['geometry'])
		#вот тут тупо метод контейнс возращал тру или фолс 
		if polygon.contains(temp_point):
			print("found one")
			output_features.append(feature)
print(output_features)
output.write(output_features)
 
    

# import json
# # depending on your version, use: from shapely.geometry import shape, Point

# # load GeoJSON file containing sectors
# with open('LNDCVR_California_State_Shape(1)/Shape/LandCover_Woodland3.geojson') as f:
#     js = json.load(f)

# # construct point based on lon/lat returned by geocoder
# print("we're starting...")
# # check each polygon to see if it contains the point
# for feature in js['features']:
