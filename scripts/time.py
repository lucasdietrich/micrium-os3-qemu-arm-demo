import datetime

utc_time = int(datetime.datetime.now().timestamp())

micrium_time = int(714418249)
micrium_ref = int(datetime.datetime(year=2000, month=1, day=1, hour=0, minute=0, second=0).timestamp())
# micrium_date = int(datetime.datetime(year=2022, month=8, day=21, hour=17, minute=31, second=0).timestamp())

print(utc_time, micrium_time + micrium_ref)

# diff = utc_time - micrium_time
# print(f"now={utc_time} micrium={micrium_time} utc={utc_time} diff={diff}")
# print(diff/3600*24/365.25)