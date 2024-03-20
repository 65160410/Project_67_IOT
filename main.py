from ultralytics import YOLO


model = YOLO("best7.pt")  # load a pretrained model (recommended for training)

res = model.predict(source=0, show=True)
