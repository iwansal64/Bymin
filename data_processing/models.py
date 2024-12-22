class HealthData:
    def __init__(self, health_data: dict):
        self.health_data = health_data
        self.type = "Health Data"
        if "type" in self.health_data:
            self.type = self.health_data["type"]
            del self.health_data["type"]
        
        if "date" in self.health_data:
            try:
                self.health_data["time"] = self.health_data["date"].split(" - ")[1]
                self.health_data["date"] = self.health_data["date"].split(" - ")[0]
            except IndexError:
                pass
