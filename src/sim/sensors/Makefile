PROTOC = protoc

ROOTPATH = ../../..
PROTOS_PATH = $(ROOTPATH)/protos

proto:
	$(PROTOC) -I $(PROTOS_PATH) --python_out=$(ROOTPATH)/protos/ $(PROTOS_PATH)/*.proto

clean:
	rm -f *_pb2.py *.pyc
