from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn
import time
import asyncio
from googletrans import Translator

app = FastAPI(title="VapeCloud Translation Service")
translator = Translator()

request_counter = 0

class TranslateRequest(BaseModel):
    text: str
    target: str = "ru"

class TranslateResponse(BaseModel):
    id: int = 0
    success: bool
    translation: str = ""
    error: str = ""
    elapsed_ms: float = 0.0

@app.post("/translate", response_model=TranslateResponse)
async def translate(request: TranslateRequest):
    global request_counter
    request_counter += 1
    current_id = request_counter

    start_time = time.time()

    if not request.text or not request.text.strip():
        return TranslateResponse(
            id=current_id,
            success=False,
            error="Empty text"
        )

    try:
        result = await asyncio.to_thread(
            translator.translate,
            request.text,
            dest=request.target
        )

        elapsed_ms = (time.time() - start_time) * 1000

        return TranslateResponse(
            id=current_id,
            success=True,
            translation=result.text,
            elapsed_ms=round(elapsed_ms, 2)
        )

    except Exception as e:
        elapsed_ms = (time.time() - start_time) * 1000
        return TranslateResponse(
            id=current_id,
            success=False,
            error=str(e),
            elapsed_ms=round(elapsed_ms, 2)
        )

@app.get("/health")
async def health():
    return {"status": "ok", "translator": "googletrans", "requests": request_counter}
