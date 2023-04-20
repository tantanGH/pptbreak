import os

for i in range(1):
  id = f"{(i+1):02d}"
  with open(f"data/MUSIC{id}.PCM","rb") as f:
    data = f.read()
    print(f"""
  .align 2
  .globl _adpcm_music{id}
  .data

_adpcm_music{id}:
""")
    for j in range(0, len(data), 16):
      line = "  .dc.b " + ",".join(["$" + format(b, "02x") for b in data[j:j+16]])
      print(line)
