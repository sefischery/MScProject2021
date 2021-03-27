FROM python:3.9.2-slim-buster

# Define the working directory in the container
WORKDIR /app
# Copy and install the requirements.
COPY requirements.txt /app/requirements.txt
RUN pip3 install -r requirements.txt

#COPY entire source
COPY . .
# Define environment variables
ENV dash_port=8050
ENV dash_debug="True"

CMD ["python3", "main_website.py"]