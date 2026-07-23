import httpx
import asyncio

async def test_translate():
    async with httpx.AsyncClient() as client:
        response = await client.post(
            "http://127.0.0.1:8000/translate",
            json={"text":  "👋 Welcome to VapeCloud!\n\n"
        "I'm your personal assistant for buying e-cigarettes, vapes, and liquids.\n\n"
        "🛒 What can I do for you?\n"
        "• Show catalog — /catalog\n"
        "• View your cart — /cart\n"
        "• See your orders — /orders\n"
        "• Get help — /help\n\n"
        "Let's get started! 🚀", "target": "s"}
        )
        print(response.json())

if __name__ == "__main__":
    asyncio.run(test_translate())
