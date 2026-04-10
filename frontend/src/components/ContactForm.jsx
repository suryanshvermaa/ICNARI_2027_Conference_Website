import React, { useState } from 'react';

const ContactForm = () => {
  const [formData, setFormData] = useState({
    name: '',
    email: '',
    subject: '',
    phone: '',
    message: '',
  });

  const handleChange = (e) => {
    setFormData((prev) => ({
      ...prev,
      [e.target.name]: e.target.value,
    }));
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const res = await fetch(`${import.meta.env.VITE_API_URL}/api/v1/contact`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(formData),
      });

      const result = await res.json();
      alert(result.message);

      // Clear the form
      setFormData({
        name: '',
        email: '',
        subject: '',
        phone: '',
        message: '',
      });
    } catch (err) {
      console.error(err);
      alert('Something went wrong. Please try again.');
    }
  };

  return (
    <div className="site-card max-w-md mx-auto mt-12 p-8">
      <h2 className="text-3xl font-bold text-center text-zinc-900 dark:text-slate-50 mb-2">Keep in touch</h2>
      <div className="w-16 h-1 bg-indigo-500 mx-auto mb-4 rounded" />
      <form onSubmit={handleSubmit} className="space-y-4">
        <input
          type="text"
          name="name"
          placeholder="Your name"
          value={formData.name}
          onChange={handleChange}
          required
          className="w-full rounded-lg border border-zinc-300 bg-white p-3 text-zinc-900 placeholder:text-zinc-400 focus:outline-none focus:ring-2 focus:ring-indigo-500/40 dark:border-slate-700 dark:bg-slate-950/40 dark:text-slate-50 dark:placeholder:text-slate-400 dark:focus:ring-indigo-400/40"
        />
        <input
          type="email"
          name="email"
          placeholder="Email"
          value={formData.email}
          onChange={handleChange}
          required
          className="w-full rounded-lg border border-zinc-300 bg-white p-3 text-zinc-900 placeholder:text-zinc-400 focus:outline-none focus:ring-2 focus:ring-indigo-500/40 dark:border-slate-700 dark:bg-slate-950/40 dark:text-slate-50 dark:placeholder:text-slate-400 dark:focus:ring-indigo-400/40"
        />
        <input
          type="text"
          name="subject"
          placeholder="Subject"
          value={formData.subject}
          onChange={handleChange}
          required
          className="w-full rounded-lg border border-zinc-300 bg-white p-3 text-zinc-900 placeholder:text-zinc-400 focus:outline-none focus:ring-2 focus:ring-indigo-500/40 dark:border-slate-700 dark:bg-slate-950/40 dark:text-slate-50 dark:placeholder:text-slate-400 dark:focus:ring-indigo-400/40"
        />
        <input
          type="tel"
          name="phone"
          placeholder="Phone"
          value={formData.phone}
          onChange={handleChange}
          className="w-full rounded-lg border border-zinc-300 bg-white p-3 text-zinc-900 placeholder:text-zinc-400 focus:outline-none focus:ring-2 focus:ring-indigo-500/40 dark:border-slate-700 dark:bg-slate-950/40 dark:text-slate-50 dark:placeholder:text-slate-400 dark:focus:ring-indigo-400/40"
        />
        <textarea
          name="message"
          placeholder="Message"
          value={formData.message}
          onChange={handleChange}
          required
          rows="4"
          className="w-full rounded-lg border border-zinc-300 bg-white p-3 text-zinc-900 placeholder:text-zinc-400 focus:outline-none focus:ring-2 focus:ring-indigo-500/40 dark:border-slate-700 dark:bg-slate-950/40 dark:text-slate-50 dark:placeholder:text-slate-400 dark:focus:ring-indigo-400/40"
        />
        <button
          type="submit"
          className="btn btn-primary w-full"
        >
          Send
        </button>
      </form>
    </div>
  );
};

export default ContactForm;
