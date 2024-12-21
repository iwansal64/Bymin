import models, datetime
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas


def create_pdf_from_health_data(health_data: models.HealthData):
    '''Creating PDF File Contains Health Data'''

    # Create a canvas to draw on
    pdf_filename = f"health_report_{datetime.datetime.now().strftime('%d-%m-%Y_%H-%M-%S')}.pdf"
    pdf_canvas = canvas.Canvas(pdf_filename, pagesize=(80*mm, 57*mm))

    # Draw text on the PDF
    pdf_canvas.drawString(75, 140, "Health Report")
    pdf_canvas.line(25, 130, 200, 130)
    pdf_canvas.setFontSize(10)
    for index, (key, value) in enumerate(health_data.health_data.items()):
        key = str(key)
        key = key.replace("-", " ").capitalize()
        
        pdf_canvas.drawString(25, 110-(10*index), str(key))
        pdf_canvas.drawString(100, 110-(10*index), ": "+str(value))

    # Save the PDF
    pdf_canvas.save()