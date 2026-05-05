Orden de ejecucion:

1) ./cajero + parametro (el parametro hace referencia al numero de cajero)
2) ./banco


Consideraciones:
- evite el evento "EVT_RTA_DEPOSITO_NOK" ya que era redundante repetir el error de cliente inexistente. 
- si la cuenta queda en 0 al extraer el monto, luego se le asigna un nuevo monto.
