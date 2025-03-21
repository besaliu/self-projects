const express = require('express');
const cors = require('cors');
const { HfInference } = require('@huggingface/inference');
require('dotenv').config();

// Initialize Express app
const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Initialize Hugging Face client
const hf = new HfInference(process.env.HUGGINGFACE_API_KEY);

// Store conversation history in memory (use Redis for production)
const conversationHistory = {}; // Keyed by session/user ID

// Chat endpoint
app.post('/api/chat', async (req, res) => {
  console.log('Chat endpoint hit');

  try {
    const { messages, sessionId } = req.body;

    if (!messages || !Array.isArray(messages) || messages.length === 0) {
      return res.status(400).json({ error: 'Invalid messages format' });
    }

    if (!conversationHistory[sessionId]) {
      conversationHistory[sessionId] = [];
    }

    conversationHistory[sessionId].push(...messages);

    const maxHistory = 5;
    if (conversationHistory[sessionId].length > maxHistory * 2) {
      conversationHistory[sessionId] = conversationHistory[sessionId].slice(-maxHistory * 2);
    }

    let prompt = conversationHistory[sessionId]
      .map(msg => `${msg.role === 'user' ? 'Human' : 'AI'}: ${msg.content}`)
      .join("\n") + "\nAI:";

    const response = await hf.textGeneration({
      model: "mistralai/Mistral-7B-Instruct-v0.1",
      inputs: prompt,
      parameters: {
        max_new_tokens: 150,
        temperature: 0.7,
        top_p: 0.95,
        repetition_penalty: 1.2
      }
    });

    let generatedText = response.generated_text.trim();

    // Ensure we remove the prompt and keep only AI's response
    if (generatedText.startsWith(prompt)) {
      generatedText = generatedText.substring(prompt.length).trim();
    }

    // Remove unnecessary "Human:" or "AI:" prefixes
    generatedText = generatedText.replace(/^(Human:|AI:)/g, '').trim();

    // Prevent redundant AI intro messages (e.g., "Hello! I'm your AI assistant.")
    const aiIntroPattern = /^Hello! I'm your AI assistant. How can I help you today\?/i;
    generatedText = generatedText.replace(aiIntroPattern, '').trim();

    // Ensure we don't return an empty response
    if (!generatedText) {
      generatedText = "I'm here to help! What would you like to know?";
    }

    conversationHistory[sessionId].push({ role: "assistant", content: generatedText });

    res.json({
      message: generatedText,
      id: Date.now().toString()
    });

  } catch (error) {
    console.error('Error in chat endpoint:', error);
    res.status(500).json({
      error: 'Failed to process chat request',
      details: error.message
    });
  }
});


// Start server
app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
